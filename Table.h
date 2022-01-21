#pragma once

#include "Server.h"
#include "Player.h"
#include "Croupier.h"

#include <map>
#include <chrono>
#include <thread>

class Table {
public:
    Table(Server* server) {
        _isRoundContinues = false;
        _maxPlayers = consts::maxPlayersPerTable;
    }

    void AddPlayer(Player p, int fd) {
        std::cout << "Adding player " << p.getName() << " to table" << std::endl;
        _players.insert({fd, p});
    }

    bool isPlayerCanBeAdded() {
        return ((_players.size() < _maxPlayers) && (!_isRoundContinues));
    }

    bool isTableEmpty() {
        return _players.size() == 0;
    }

    void startRound() {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

    void run() {
        std::cout << "Table::run" << std::endl;
        while(_players.size() > 0) {
            // main logic of round here
            std::cout << _players.size() << " players in starting round" << std::endl;
            std::cout << "\tstart round" << std::endl;
            startRound();
            std::cout << "\tend of round" << std::endl;
            // end of round
            _isRoundContinues = false;
            std::cout << "\twaiting for new players to come" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            _isRoundContinues = true;
        }
    }

private:
    Server* server_;
    std::map<int, Player> _players;
    Croupier _croupier;
    int _maxPlayers;
    bool _isRoundContinues;
};