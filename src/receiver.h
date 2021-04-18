#ifndef MAC2RPI_PLAYBACK_SERVER_RECEIVER_H
#define MAC2RPI_PLAYBACK_SERVER_RECEIVER_H

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <memory>
#include <utility>

#include "player.h"

namespace asio = boost::asio;

static constexpr std::size_t NETWORK_BUFFER_SIZE = 4096;

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
        , player_(std::move(player)) {
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
                    BOOST_LOG_TRIVIAL(error) << boost::format(
                        "error receiving data; error_code=%1%\n") % error_code;

                    // TODO: shall we treat this as an unrecoverable error?
                    //   If not, we should call do_receive again.
                } else {
                    BOOST_LOG_TRIVIAL(info) << boost::format(
                        "data received; length=%1%\n") % length;

                    player_->AddData(data_.data(), length);
                    do_receive();
                }
            }
        );
    }
};

#endif //MAC2RPI_PLAYBACK_SERVER_RECEIVER_H
