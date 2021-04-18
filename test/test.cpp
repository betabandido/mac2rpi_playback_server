#define BOOST_TEST_MODULE Test
#include <boost/test/unit_test.hpp>

#include <player.h>
#include <receiver.h>

#include <algorithm>
#include <chrono>

using namespace std::chrono_literals;

static void sender(asio::io_context& io_context, const std::vector<char>& data) {
    asio::ip::udp::endpoint endpoint(
        boost::asio::ip::make_address("239.255.0.1"),
        30001
    );
    asio::ip::udp::socket socket(io_context, endpoint.protocol());

    for (auto it = std::begin(data); it < std::end(data);) {
        auto bytes_to_send = std::min(
            std::size_t(std::end(data) - it),
            NETWORK_BUFFER_SIZE
        );
        socket.async_send_to(
            asio::buffer(&(*it), bytes_to_send),
            endpoint,
            [](auto error_code, auto length) {}
        );
        it += bytes_to_send;
    }
}

static void sender(asio::io_context& io_context, const std::string& data) {
    sender(io_context, std::vector<char>(std::begin(data), std::end(data)));
}

class TestPlayer : public Player {
public:
    std::vector<char> receivedData;

    void AddData(char* data, std::size_t length) override {
        receivedData.insert(
            std::end(receivedData),
            data,
            data + length
        );
    }

    std::string receivedDataAsString() const {
        return std::string(std::begin(receivedData), std::end(receivedData));
    }
};

BOOST_AUTO_TEST_CASE(test_receiver_receives_data)
{
    asio::io_service io_service;

    auto player = std::make_shared<TestPlayer>();

    receiver r(
        io_service,
        asio::ip::make_address("0.0.0.0"),
        asio::ip::make_address("239.255.0.1"),
        30001,
        player
    );

    std::thread sender_thread([&]() { sender(io_service, "Hello World!"); });
    sender_thread.detach();

    io_service.run_for(5s);

    BOOST_CHECK_EQUAL(player->receivedDataAsString(), "Hello World!");
}

BOOST_AUTO_TEST_CASE(test_receiver_can_receive_multiple_datagrams)
{
    asio::io_service io_service;

    auto player = std::make_shared<TestPlayer>();

    receiver r(
        io_service,
        asio::ip::make_address("0.0.0.0"),
        asio::ip::make_address("239.255.0.1"),
        30001,
        player
    );

    std::vector<char> data(8 * 1024);
    std::generate(
        std::begin(data),
        std::end(data),
        []() { return std::rand() % 255; }
    );

    std::thread sender_thread([&]() { sender(io_service, data); });
    sender_thread.detach();

    io_service.run_for(5s);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        std::begin(player->receivedData),
        std::end(player->receivedData),
        std::begin(data),
        std::end(data)
    );
}
