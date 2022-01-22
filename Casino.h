#pragma once

#include "Server.h"
#include "Table.h"
#include "Consts.h"

#include <vector>
#include <thread>
#include <string>

class Casino {
public:
    Casino() {
        _maxTables = consts::maxTables;

        _server = new Server();
        _server->init();

        for (int i = 0; i < _maxTables; ++i)
            _tables.push_back(Table(_server));
    }

    void run() {
        while(true) {
            // first we wait for player to connect and connect it actually
            int player_fd = _server->loopForWaitingNewPlayers();
            Player p;

            _server->sendMessage(player_fd, "Enter your name: ");
            std::string playerName = _server->getReply(player_fd);
            _server->sendMessage(player_fd, "Enter your balance: ");
            std::string playerBalance = _server->getReply(player_fd);
            p.setBalance(std::stoi(playerBalance));
            p.setName(playerName);

            bool playerWasAdded = false;
            // then we check if there is space in some table to sit
            for (int i = 0; i < _tables.size() && !playerWasAdded; ++i) {
                if (_tables[i].isFreeSpace()) {
                    if (!_tables[i].isRoundContinues()) { // if round is not playing - just add player to table
                        if (_tables[i].isTableEmpty()) {
                            _tables[i].AddPlayer(p, player_fd);
                            // and here start run in new thread
                            _tables_threads.push_back(std::thread(&Table::run, &_tables[i]));
                        }
                        else {
                            _tables[i].AddPlayer(p, player_fd);
                        }
                    }
                    else { // if round continues - add players to waiting
                        _tables[i].AddWaitingPlayer(p, player_fd);
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
    }

private:
    Server* _server;
    std::vector<Table> _tables;
    std::vector<std::thread> _tables_threads;
    int _maxTables;
};