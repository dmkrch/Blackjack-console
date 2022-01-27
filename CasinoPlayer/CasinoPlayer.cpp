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
        int bet;
        // send bet and check if not more than balance to opened socket
        std::cin >> bet;
        while(bet > _player.getBalance()) {
            std::cout << "Your balance: " << _player.getBalance() << "$ is less" << std::endl;
            std::cout << "Enter right bet: ";
            std::cin >> bet;
        }
        _player.setBet(bet);
        _player.setBalance(_player.getBalance() - bet);
        _client.sendMessage(std::to_string(bet));


        // get info about self' and others' cards
        printReply();

        std::string temp = "t";
        std::string reply = "";
        std::string turn;

        while(reply != "pass") {
            _client.sendMessage(temp); // this because 2 replies can't be byside
            printReply(); // prints 1. take 2. pass
            std::cin >> turn;
            _client.sendMessage(turn);
            printReply(); // prints // you've taken card... your cards are... you have 21 and u pass..
            _client.sendMessage(temp); // this because 2 replies can't be byside
            reply = _client.getReply(); // pass or play
        }
    }



    if (_player.getBalance() == 0) {
        std::cout << "you lost all money!" << std::endl;
    }
}

void CasinoPlayer::printReply() {
    std::cout << _client.getReply(); 
    std::cout.flush();
}

int CasinoPlayer::getMsgId(std::string str) {

}

void CasinoPlayer::stopPlaying() {
    _client.disconnect();
}