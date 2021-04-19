#ifndef MAC2RPI_PLAYBACK_SERVER_DEFAULT_PLAYER_H
#define MAC2RPI_PLAYBACK_SERVER_DEFAULT_PLAYER_H

#include "player.h"

#include <boost/lockfree/spsc_queue.hpp>
#include <memory>

#include <portaudiocpp/PortAudioCpp.hxx>

namespace pa = portaudio;

static constexpr size_t RING_BUFFER_SIZE = 64 * 1024;

class DefaultPlayer : public Player {
public:
    DefaultPlayer();

    void AddData(
        char *data,
        std::size_t length
    ) override;

    void Play();

private:
    boost::lockfree::spsc_queue<
        char,
        boost::lockfree::capacity<RING_BUFFER_SIZE>
    > ring_buffer_;

    std::unique_ptr<pa::BlockingStream> stream_;
};

#endif //MAC2RPI_PLAYBACK_SERVER_DEFAULT_PLAYER_H
