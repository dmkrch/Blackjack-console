#pragma once

#include "Client.hpp"
#include "Player.hpp"

class CasinoPlayer {
public:
    CasinoPlayer();
    ~CasinoPlayer();
    void run();

private:
    void stopPlaying();
    void printReply();
    Client _client;
    Player _player;
};

