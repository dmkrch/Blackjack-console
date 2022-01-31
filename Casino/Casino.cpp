#include "Casino.hpp"


Casino::Casino() {
    _maxTables = consts::maxTables;

    _server = std::make_shared<Server>();
    _server->init();

    for (int i = 0; i < _maxTables; ++i)
        _tables.emplace_back(Table(_server));
}

void Casino::run() {
    while(true) {
        // first we wait for player to connect and connect it actually
        int player_fd = _server->handleNewConnection();
        Player p;

        std::string playerName;
        std::string playerBalance;
        bool isSuccessfullyAdded = true;

        try {
            _server->sendMessage(player_fd, "Enter your name: ");
            playerName = _server->getReply(player_fd);
            _server->sendMessage(player_fd, "Enter your balance: ");
            playerBalance = _server->getReply(player_fd);
        }
        catch(const Exception& ex) {
            std::cout << ex.what() << std::endl;
            isSuccessfullyAdded = false;
        }
        
        if (isSuccessfullyAdded) {
            p.setBalance(std::stoi(playerBalance));
            p.setName(playerName);

            bool playerWasAdded = false;
            // then we check if there is space in some table to sit
            for (int i = 0; i < _tables.size() && !playerWasAdded; ++i) {
                if (_tables[i].isFreeSpace()) {
                    if (!_tables[i].isRoundContinues()) {
                        if (_tables[i].isTableEmpty() && !_tables[i].wasRunned()) {
                            _tables[i].addPlayer(p, player_fd);
                            // and here start run in new thread
                            _tables_threads.emplace_back(std::thread(&Table::run, &_tables[i]));
                        }
                        else {
                            _tables[i].addPlayer(p, player_fd);
                        }
                    }
                    else { // if round continues - add players to waiting
                        _tables[i].addWaitingPlayer(p, player_fd);
                    }
                    playerWasAdded = true;
                }
            }
            if (!playerWasAdded) {
                // disconnect him here
                _server->sendMessage(player_fd, "\nSorry, all tables are full :(\n");
                _server->closeConnection(player_fd);
            }
        }
        else   
            std::cout << "couldnt add new player" << std::endl;
    }
}
