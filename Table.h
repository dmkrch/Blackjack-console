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
        _tableId = _counter++;

        _isRoundContinues = false;
        _maxPlayers = consts::maxPlayersPerTable;
        _server = server;
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
        printLog(std::to_string(_players.size()) + " players in starting room");
        
        std::string names;
        for (auto pl : _players) {
            names += pl.second.getName() + " ";
        }
        printLog("names: " + names);

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

    void printLog(std::string msg) {
        std::cout << "Table #" << _tableId << ": " << msg << std::endl;
    }

    void run() {
        printLog("run()");
        while(_players.size() > 0) {
            // main logic of round here
            printLog("start of round");

            startRound();
            printLog("end of round");

            if (_players.size() < _maxPlayers) {
                _isRoundContinues = false;
                printLog("waiting for new players...");
                std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                _isRoundContinues = true;
            }
        }
    }

    void sendMsgToAllPlayers(const char* msg) {
        for (auto pair : _players) {
            _server->sendMessage(pair.first, msg);
        }
    }

private:
    int _tableId;
    static int _counter;
    Server* _server;
    std::map<int, Player> _players;
    std::map<int, Player> _waitingPlayers;
    Croupier _croupier;
    int _maxPlayers;
    bool _isRoundContinues;
};

int Table::_counter = 1;