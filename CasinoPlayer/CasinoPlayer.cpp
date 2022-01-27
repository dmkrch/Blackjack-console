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
        // here we can get either 'Wait', or 'Your turn!'
        _client.sendMessage(temp); // this because 2 replies can't be byside
        std::string yourTurnMsg = _client.getReply(); // <0> message FROM casino || Wait
        std::cout << yourTurnMsg;
        if (yourTurnMsg != "Your turn!\n") {
            _client.sendMessage(temp);
            std::cout << _client.getReply();
        }

        std::string passOrPlay = "";
        std::string playerChoice;

        // get info about taking or pass
        while(passOrPlay != "pass") {
            _client.sendMessage(temp); // this because 2 replies can't be byside
            printReply(); // <1> message FROM casino
            std::cin >> playerChoice;
            _client.sendMessage(playerChoice); // <2> message TO casino
            printReply(); // <3> message FROM casino
            _client.sendMessage(temp); // this because 2 replies can't be byside
            passOrPlay = _client.getReply(); // <4> message FROM casino
        }


        // get msg about player cards in end of round
        _client.sendMessage(temp);
        std::cout << _client.getReply(); // <5> message FROM casino
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