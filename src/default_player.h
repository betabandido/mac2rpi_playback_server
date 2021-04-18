#ifndef MAC2RPI_PLAYBACK_SERVER_DEFAULT_PLAYER_H
#define MAC2RPI_PLAYBACK_SERVER_DEFAULT_PLAYER_H

#include "player.h"

class DefaultPlayer : public Player {
public:
    void AddData(
        char *data,
        std::size_t length
    ) override {}
};

#endif //MAC2RPI_PLAYBACK_SERVER_DEFAULT_PLAYER_H
