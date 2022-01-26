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
    _client.sendMessage(name);

    // get put balance
    printReply();
    std::cin >> balance;
    _player.setBalance(stoi(balance));
    _client.sendMessage(balance);

    while(_player.getBalance() > 0) {
        // get start round and bet info
        printReply();

        // get bet info
        std::string bet;
        // send bet and check if not more than balance to opened socket
        std::cin >> bet;
        while(std::stoi(bet) > _player.getBalance()) {
            std::cout << "Your balance: " << _player.getBalance() << "$ is less" << std::endl;
            std::cout << "Enter right bet: ";
            std::cin >> bet;
        }
        _player.setBet(stoi(bet));
        _player.setBalance(_player.getBalance() - stoi(bet));
        _client.sendMessage(bet);
    }

    if (_player.getBalance() == 0) {
        std::cout << "you lost all money!" << std::endl;
    }
}

void CasinoPlayer::printReply() {
    std::cout << _client.getReply(); 
      std::cout.flush();
}