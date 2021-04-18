#define BOOST_TEST_MODULE Test
#include <boost/test/unit_test.hpp>

#include <player.h>
#include <receiver.h>

#include <chrono>

using namespace std::chrono_literals;

static void sender(asio::io_context& io_context) {
    asio::ip::udp::endpoint endpoint(
        boost::asio::ip::make_address("239.255.0.1"),
        30001
    );
    asio::ip::udp::socket socket(io_context, endpoint.protocol());

    socket.async_send_to(
        asio::buffer(std::string("Hello World!")),
        endpoint,
        [](auto error_code, auto length) {}
    );
}

class TestPlayer : public Player {
public:
    std::string receivedData;

    void AddData(char* data, std::size_t length) override {
        receivedData = std::string(data, length);
    }
};

BOOST_AUTO_TEST_CASE(my_test)
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

    std::thread sender_thread([&]() { sender(io_service); });
    sender_thread.detach();

    io_service.run_for(5s);

    BOOST_CHECK_EQUAL(player->receivedData, "Hello World!");
}
