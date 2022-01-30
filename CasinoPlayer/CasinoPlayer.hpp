#pragma once

#include "Client.hpp"
#include "Player.hpp"

class CasinoPlayer {
public:
    CasinoPlayer();
    void run();

private:
    void stopPlaying();
    void printReply() const;

private:
    Client _client;
    Player _player;
};

