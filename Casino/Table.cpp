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

void Table::throwAwayPreviousCards() {
    for (auto& pl : _players) {
        if (pl.second.getCardsAmount() > 0)
            pl.second.throwCards();
    }

    if (_croupier.getCardsAmount() > 0)
        _croupier.throwCards();
}


std::string Table::getCardsInfoForPlayer(std::pair<int, Player> playerToSend) {
    int fd = playerToSend.first;
    // cards of all players first
    std::stringstream ss;
    ss << std::endl;

    for (auto otherPlayer : _players) {
        // take info about other players' cards
        if (playerToSend.first != otherPlayer.first)
            ss << otherPlayer.second.getName() << " cards: " << otherPlayer.second.getCardsStr() << "  ";
    }

    // take info about cards of croupier
    ss << std::endl << "croupier cards: " << _croupier.getStartRoundCardsStr() << std::endl;

    // take info about cards of certain player
    ss << "your cards: " << playerToSend.second.getCardsStr() << std::endl;

    return ss.str();
}


void Table::startRound() {
    // some init before starting round
    ++_roundNumber;
    throwAwayPreviousCards();

    // log for server
    printLog(std::to_string(_players.size()) + " players in round #" + std::to_string(_roundNumber));

    // send start round info and make bet
    std::stringstream ss;
    ss << "----------------ROUND #" << _roundNumber << "--------------" << std::endl << std::endl;
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
    for (auto pl : _players) {
        std::string cardsInfo = getCardsInfoForPlayer(pl);

        // now send info about other players' cards, croupier and his(pl) cards to pl
        _server->sendMessage(pl.first, cardsInfo);
    }

    // now we need to check if someone has blackjack
    
    // // now we need to bypass every player and ask if he takes card or not
    // for (auto pl : _players) {
    //     std::string askTakeOrPass = "Do you want to take card or pass?";
    //     askTakeOrPass += "\n";
    //     askTakeOrPass += "1. take   2. pass";

    // }
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
            std::this_thread::sleep_for(std::chrono::milliseconds(consts::waitingSeconds * 1000));
            _isRoundContinues = true;
        }
    }
}

void Table::sendMsgToAllPlayers(std::string msg) {
    for (auto pair : _players) {
        _server->sendMessage(pair.first, msg);
    }
}