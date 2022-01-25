#pragma once

#include "Client.hpp"
#include "Player.hpp"

class CasinoPlayer {
public:
    CasinoPlayer();

    void run();

private:
    void printReply();
    Client _client;
    Player _player;
};


