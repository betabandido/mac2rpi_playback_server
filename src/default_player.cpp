#include "default_player.h"

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

static constexpr double SAMPLE_RATE = 44100.0;
static constexpr unsigned NUM_CHANNELS = 2;
static constexpr bool INTERLEAVED_STREAM = true;
// frame size = 2 channels x 32-bit per sample = 8 bytes
static constexpr unsigned FRAME_SIZE = 4 * NUM_CHANNELS;

static constexpr size_t NETWORK_BUFFER_SIZE = 4096;
static constexpr size_t RING_BUFFER_LOW_THRESHOLD = 16 * 1024;
static constexpr size_t MAX_FRAMES = 1024;

std::unique_ptr<portaudio::BlockingStream> createAudioStream() {
    auto& system = pa::System::instance();

    pa::DirectionSpecificStreamParameters out_params(
        system.defaultOutputDevice(),
        NUM_CHANNELS,
        pa::FLOAT32,
        INTERLEAVED_STREAM,
        system.defaultOutputDevice().defaultHighOutputLatency(),
        nullptr
    );

    pa::StreamParameters params(
        pa::DirectionSpecificStreamParameters::null(),
        out_params,
        SAMPLE_RATE,
        paFramesPerBufferUnspecified,
        paClipOff
    );

    return std::make_unique<pa::BlockingStream>(params);
}

DefaultPlayer::DefaultPlayer() : stream_(createAudioStream()) {}

void DefaultPlayer::AddData(
    char *data,
    std::size_t length
) {
    ring_buffer_.push(data, length);
}

void DefaultPlayer::Play() {
    try {
        BOOST_LOG_TRIVIAL(info) << "Player started\n";

        std::array<char, MAX_FRAMES * FRAME_SIZE> buffer{};

        while (true) {
            while (ring_buffer_.read_available() < RING_BUFFER_LOW_THRESHOLD) {
                if (stream_->isActive())
                    stream_->stop();
                std::this_thread::sleep_for(1s);
            }

            if (stream_->isStopped())
                stream_->start();

            ring_buffer_.pop(begin(buffer), MAX_FRAMES * FRAME_SIZE);
            stream_->write(buffer.data(), MAX_FRAMES);
        }

        // TODO: this code cannot be reached.
        stream_->stop();
        stream_->close();
    } catch (const std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << boost::format("Exception in thread: %1%\n") % e.what();
    }
}
