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
    Table(std::shared_ptr<Server> server);
    
    void run();

    void addPlayer(Player p, int fd);
    void addWaitingPlayer(Player p, int fd);

    bool isRoundContinues();
    bool isTableEmpty();
    bool isFreeSpace();
    bool wasRunned();

private:
    ShoeDeck _shoeDeck;
    
    void startRound();
    void printLog(const std::string& msg);
    void sendMsgToAllPlayers(std::string msg);

    //gameplay:
    void throwAwayPreviousCards();
    std::string getCardsInfoOfAll(const std::pair<int, Player>& eachPlayer);
    std::string getCardsInfoOfOtherPlayers(const std::pair<int, Player>& pl);
    void setPlayersState();
    void callDelay();

    std::string getPlayerResultAndSend(Player& pl1, int fd);

    std::shared_ptr<Server> _server;
    std::map<int, Player> _players;
    std::map<int, Player> _waitingPlayers;
    Croupier _croupier;

    int _tableId;
    int _roundNumber;
    static int _roundCounter;
    int _maxPlayers;
    bool _isRoundContinues;
    bool _wasRunned;
};