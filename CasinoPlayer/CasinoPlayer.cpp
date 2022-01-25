#include "CasinoPlayer.hpp"

CasinoPlayer::CasinoPlayer() { }

void CasinoPlayer::run() {
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
    }
}

void CasinoPlayer::printReply() {
    std::cout.flush();
    std::cout << _client.getReply(); 
    std::cout.flush();
}