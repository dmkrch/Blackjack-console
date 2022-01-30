#include "CasinoPlayer.hpp"

CasinoPlayer::CasinoPlayer() { }

void CasinoPlayer::run() {
    char buffer[BUFSIZE];

    std::string name;
    std::string balance;
    std::string bet;

    // get 'put name'
    printReply();
    std::cin >> name;

    _client.sendMessage(name);

    // get put balance
    printReply();
    std::cin >> balance;
    _player.setBalance(stoi(balance));

    _client.sendMessage(balance);

    while(_player.getBalance() > 0) {
        printReply(); // get your balance msg
        _client.sendMessage("t0");


        // get start round and bet info
        printReply();

        // get bet info
        int bet;
        std::cin >> bet;
        _player.setBet(bet);
        _player.setBalance(_player.getBalance() - bet);

        _client.sendMessage(std::to_string(bet));
        // get info about self' and others' cards
        printReply();

        // getting 'Your turn!'
        _client.sendMessage("t1"); // this because 2 replies can't be byside

        printReply(); // <1> message FROM casino

        std::string passOrPlay = "";
        std::string playerChoice;

        // get info about taking or pass
        while(passOrPlay != "pass") {
            _client.sendMessage("t2"); // this because 2 replies can't be byside
            
            printReply(); // <1> message FROM casino
        
            std::cin >> playerChoice;
            _client.sendMessage(playerChoice); // <2> message TO casino
            printReply(); // <3> message FROM casino
            _client.sendMessage("t3"); // this because 2 replies can't be byside
            passOrPlay = _client.getReply(); // <3, 4> message FROM casino
        }


        // get msg about player cards in end of round
        _client.sendMessage("t3");
        std::cout << _client.getReply(); // <5> message FROM casino

        std::string croupierCards = "";
        while(croupierCards != "Croupier cards: ") { // <6, 7> message FROM casino
            _client.sendMessage("t4");
            croupierCards = _client.getReply();
            std::cout << croupierCards;
        }

        // get msg "croupier cards 7 8 9..."
        _client.sendMessage("t5"); // this because 2 replies can't be byside
        std::cout << _client.getReply(); // <8> message

        // get msg "croupier passes"
        _client.sendMessage("t6"); // this because 2 replies can't be byside
        std::cout << _client.getReply(); // <9> message

        // get msg "RESULTS"
        _client.sendMessage("t7"); // this because 2 replies can't be byside
        std::cout << _client.getReply(); // <10> message    

        // get actual results
        _client.sendMessage("t8"); // this because 2 replies can't be byside
        std::cout << _client.getReply(); // <11> message    


        _client.sendMessage("t9"); // this because 2 replies can't be byside
    }

    if (_player.getBalance() == 0) {
        std::cout << "you lost all money!" << std::endl;
        stopPlaying();
    }
}

void CasinoPlayer::printReply() const {
    std::cout << _client.getReply(); 
    fflush(stdout);
}


void CasinoPlayer::stopPlaying() {
    _client.disconnect();
}