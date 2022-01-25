#pragma once

#include "Client.hpp"
#include "Player.hpp"

class CasinoPlayer {
public:
    CasinoPlayer();

    void run() {
        char buffer[BUFSIZE];

        std::string name;
        std::string balance;
        std::string bet;

        // get put name
        printReply();
        std::cin >> name;
        _client.sendMessage(name.c_str());

        // get put balance
        printReply();
        std::cin >> balance;
        _client.sendMessage(balance.c_str());

        while(true) {
            // get start round info
            printReply();

            // get msg about making a bet
            printReply();
            std::cin >> bet;
            _client.sendMessage(balance.c_str());
        }
    }

private:
    void printReply() { 
        fflush(stdout);
        std::cout << _client.getReply(); 
    }
    Client _client;
    Player _player;
};


