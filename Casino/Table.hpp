#pragma once

#include "Server.hpp"
#include "Player.hpp"
#include "Croupier.hpp"

#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include <sstream>


class Table {
public:
    Table(Server* server);
    
    void run();

    void AddPlayer(Player p, int fd);
    void AddWaitingPlayer(Player p, int fd);

    bool isRoundContinues();
    bool isTableEmpty();
    bool isFreeSpace();

private:
    void startRound();
    void printLog(std::string msg);
    void sendMsgToAllPlayers(std::string msg);

    int _tableId;
    int _roundNumber;
    static int _counter;
    Server* _server;
    std::map<int, Player> _players;
    std::map<int, Player> _waitingPlayers;
    Croupier _croupier;
    int _maxPlayers;
    bool _isRoundContinues;
};