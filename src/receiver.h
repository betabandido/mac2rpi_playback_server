#ifndef MAC2RPI_PLAYBACK_SERVER_RECEIVER_H
#define MAC2RPI_PLAYBACK_SERVER_RECEIVER_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <memory>

#include "player.h"

namespace asio = boost::asio;

static constexpr size_t NETWORK_BUFFER_SIZE = 4096;

class receiver {
public:
    receiver(
        asio::io_context &io_context,
        const asio::ip::address &listen_address,
        const asio::ip::address &multicast_address,
        unsigned short multicast_port,
        std::shared_ptr<Player> player
    )
        : socket_(io_context)
        , player_(player) {
        auto listen_endpoint = asio::ip::udp::endpoint(
            listen_address,
            multicast_port
        );

        socket_.open(listen_endpoint.protocol());
        socket_.set_option(asio::ip::udp::socket::reuse_address(true));
        socket_.bind(listen_endpoint);

        socket_.set_option(
            asio::ip::multicast::join_group(multicast_address)
        );

        do_receive();
    }

private:
    asio::ip::udp::socket socket_;
    asio::ip::udp::endpoint sender_endpoint_;
    std::array<char, NETWORK_BUFFER_SIZE> data_;
    std::shared_ptr<Player> player_;

    void do_receive() {
        socket_.async_receive_from(
            asio::buffer(data_),
            sender_endpoint_,
            [this](
                boost::system::error_code error_code,
                std::size_t length
            ) {
                if (error_code) {
                    std::cerr << "error receiving data: " << error_code << std::endl;
                } else {
                    player_->AddData(data_.data(), length);
                    do_receive();
                }
            }
        );
    }
};

#endif //MAC2RPI_PLAYBACK_SERVER_RECEIVER_H
