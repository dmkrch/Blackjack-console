#pragma once

#include "Server.hpp"
#include "Player.hpp"
#include "Croupier.hpp"
#include "ShoeDeck.hpp"
#include "CardDeck.hpp"
#include "Consts.hpp"

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
    ShoeDeck _shoeDeck;

    
    void startRound();
    void printLog(std::string msg);
    void sendMsgToAllPlayers(std::string msg);

    //gameplay:
    void throwAwayPreviousCards();
    std::string getCardsInfoForPlayer(std::pair<int, Player> eachPlayer);

    Server* _server;
    std::map<int, Player> _players;
    std::map<int, Player> _waitingPlayers;
    Croupier _croupier;

    int _tableId;
    int _roundNumber;
    static int _roundCounter;
    int _maxPlayers;
    bool _isRoundContinues;
};