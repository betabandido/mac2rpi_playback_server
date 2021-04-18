#ifndef MAC2RPI_PLAYBACK_SERVER_PLAYER_H
#define MAC2RPI_PLAYBACK_SERVER_PLAYER_H

class Player {
public:
    virtual ~Player() = default;

    virtual void AddData(char* data, std::size_t length) = 0;
};

#endif //MAC2RPI_PLAYBACK_SERVER_PLAYER_H
