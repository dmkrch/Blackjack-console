#pragma once

#include "Client.hpp"
#include "Player.hpp"

class CasinoPlayer {
public:
    CasinoPlayer();
    ~CasinoPlayer() {
        std::cout << "~CasinoPlayer" << std::endl;
        close(_client.getClientSocket());
    }
    void run();

private:
    void printReply();
    Client _client;
    Player _player;
};

