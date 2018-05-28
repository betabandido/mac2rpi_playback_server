#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/lockfree/spsc_queue.hpp>

#include <portaudiocpp/PortAudioCpp.hxx>

using boost::asio::ip::tcp;
namespace asio = boost::asio;
using namespace std::chrono_literals;
namespace pa = portaudio;

static constexpr unsigned short PORT = 19999;

static constexpr double SAMPLE_RATE = 44100.0;
static constexpr unsigned NUM_CHANNELS = 2;
static constexpr bool INTERLEAVED_STREAM = true;
// frame size = 2 channels x 32-bit per sample = 8 bytes
static constexpr unsigned FRAME_SIZE = 4 * NUM_CHANNELS;

static constexpr size_t NETWORK_BUFFER_SIZE = 4096;
static constexpr size_t RING_BUFFER_SIZE = 64 * 1024;
static constexpr size_t RING_BUFFER_LOW_THRESHOLD = 16 * 1024;
static constexpr size_t MAX_FRAMES = 1024;

static boost::lockfree::spsc_queue<
  char,
  boost::lockfree::capacity<RING_BUFFER_SIZE>> 
ring_buffer;

static std::unique_ptr<pa::BlockingStream> create_audio_stream() {
  auto& system = pa::System::instance();

  pa::DirectionSpecificStreamParameters out_params(
      system.defaultOutputDevice(),
      NUM_CHANNELS,
      pa::FLOAT32,
      INTERLEAVED_STREAM,
      system.defaultOutputDevice().defaultHighOutputLatency(),
      nullptr);

  pa::StreamParameters params(
      pa::DirectionSpecificStreamParameters::null(),
      out_params,
      SAMPLE_RATE,
      paFramesPerBufferUnspecified,
      paClipOff);

  return std::make_unique<pa::BlockingStream>(params);
}

static void player() {
  try {
    std::cout << "Player started\n";

    auto stream = create_audio_stream();
    std::array<char, MAX_FRAMES * FRAME_SIZE> buffer;

    while (true) {
      while (ring_buffer.read_available() < RING_BUFFER_LOW_THRESHOLD) {
        if (stream->isActive())
          stream->stop();
        std::this_thread::sleep_for(1s);
      }

      if (stream->isStopped())
        stream->start();

      ring_buffer.pop(begin(buffer), MAX_FRAMES * FRAME_SIZE);
      stream->write(buffer.data(), MAX_FRAMES);
    }

    stream->stop();
    stream->close();
  } catch (const std::exception& e) {
    std::cerr << boost::format("Exception in thread: %1%\n") % e.what();
  }
}

static void session(tcp::socket socket) {
  try {
    std::cout << "Session started\n";

    while (true) {
      std::array<char, NETWORK_BUFFER_SIZE> buffer;
      boost::system::error_code ec;
      auto length = asio::read(
          socket,
          asio::buffer(buffer, NETWORK_BUFFER_SIZE), 
          asio::transfer_at_least(NETWORK_BUFFER_SIZE),
          ec);
      // TODO what if length > 0 && eof ??
      if (ec == asio::error::eof)
        break; // connection closed cleanly by peer
      else if (ec)
        throw boost::system::system_error(ec);

      ring_buffer.push(begin(buffer), end(buffer));
    }
  } catch (const std::exception& e) {
    std::cerr << boost::format("Exception in thread: %1%\n") % e.what();
  }
}

static void server(asio::io_service& io_service, unsigned short port) {
  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
  std::cout << "Server started\n";
  while (true) {
    tcp::socket socket(io_service);
    acceptor.accept(socket);
    std::thread(session, std::move(socket)).detach();
  }
}

int main() {
  try {
    pa::AutoSystem auto_system;
    asio::io_service io_service;

    std::thread player_thread(player);
    player_thread.detach();

    server(io_service, PORT);
  } catch (const std::exception& e) {
    std::cerr << boost::format("Exception: %1%\n") % e.what();
  }

  return 0;
}
