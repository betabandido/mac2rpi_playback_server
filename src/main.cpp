#include <iostream>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include "default_player.h"
#include "receiver.h"

int main() {
    try {
        asio::io_service io_service;

        auto player = std::make_shared<DefaultPlayer>();

        receiver r(
            io_service,
            asio::ip::make_address("0.0.0.0"),
            asio::ip::make_address("239.255.0.1"),
            30001,
            player
        );

        BOOST_LOG_TRIVIAL(info) << "Listening";
        io_service.run();
        BOOST_LOG_TRIVIAL(warning) << "Shutting down";
    } catch (const std::exception &e) {
        BOOST_LOG_TRIVIAL(error) << boost::format("Exception: %1%") % e.what();
    }

    return 0;
}
