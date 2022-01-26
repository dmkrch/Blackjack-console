#include "Table.hpp"


std::mutex addPlayerMutex;
std::mutex addWaitPlayerMutex;
int Table::_roundCounter = 1;

Table::Table(Server* server) : _shoeDeck{ShoeDeck(consts::shoeDecks)} {
    _tableId = _roundCounter++;

    _isRoundContinues = false;
    _maxPlayers = consts::maxPlayersPerTable;
    _server = server;
    _roundNumber = 0;
}

void Table::AddPlayer(Player p, int fd) {
    printLog("Adding player " + p.getName());
    //const std::lock_guard<std::mutex> lock(addWaitPlayerMutex);
    _players.insert({fd, p});
}

void Table::AddWaitingPlayer(Player p, int fd) {
    printLog("Adding waiting player " + p.getName());
    //const std::lock_guard<std::mutex> lock(addPlayerMutex);
    _waitingPlayers.insert({fd, p});
}

bool Table::isRoundContinues() {
return _isRoundContinues;
}

bool Table::isTableEmpty() {
    return _players.size() == 0;
}

bool Table::isFreeSpace() {
    return ((_players.size() + _waitingPlayers.size()) < _maxPlayers);
}

void Table::startRound() {
    for (auto pl : _players) {
        if (pl.second.getCardsAmount() > 0)
            pl.second.throwCards();
    }

    if (_croupier.getCardsAmount() > 0)
        _croupier.throwCards();

    ++_roundNumber;
    printLog(std::to_string(_players.size()) + " players in round #" + std::to_string(_roundNumber));

    std::stringstream ss;
    ss << "Welcome to round #" << std::to_string(_roundNumber) << std::endl;
    ss << "Enter your bet for round: ";
    sendMsgToAllPlayers(ss.str());
    
    for (auto player : _players) {
        int fd = player.first;
        std::string bet = _server->getReply(fd);
        std::cout.flush();
        std::cout << "\tplayer " << player.second.getName() << " made bet " << bet << std::endl;
    }

    // now when all bets are done, giving everyone 2 cards
    for (auto& p : _players) {
        Card card = _shoeDeck.getTopCard();
        p.second.putCard(card);
        card = _shoeDeck.getTopCard();
        p.second.putCard(card);
    }

    // giving croupier 2 cards
    Card card = _shoeDeck.getTopCard();
    _croupier.putCard(card);
    card = _shoeDeck.getTopCard();
    _croupier.putCard(card);


    // now we need to send the information to all clients
    for (auto p : _players) {
        int fd = p.first;
        // cards of all players first
        // cards of croupier
        // cards of certain player
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void Table::printLog(std::string msg) {
    std::cout.flush();
    std::cout << "Table #" << _tableId << ": " << msg << std::endl;
}

void Table::run() {
    printLog("run()");
    while(_players.size() > 0) {
        // main logic of round here
        printLog("start of round");
        _isRoundContinues = true;
        startRound();
        _isRoundContinues = false;
        printLog("end of round");
        
        // if there are waiting players - time to add them to actual players
        if (_waitingPlayers.size() > 0) {
            // we should add waiting players to table if they exist
            for (auto p:_waitingPlayers)
                printLog("Adding waiting player" + p.second.getName() + " to round");

            _players.insert(_waitingPlayers.begin(), _waitingPlayers.end());
            // and clean added waiting_players
            _waitingPlayers.clear();
        }

        // now let new players connect if needed 
        if (_players.size() < _maxPlayers) {
            _isRoundContinues = false;
            printLog("waiting for new players...");
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            _isRoundContinues = true;
        }
    }
}

void Table::sendMsgToAllPlayers(std::string msg) {
    for (auto pair : _players) {
        _server->sendMessage(pair.first, msg.c_str());
    }
}