#include "Table.hpp"

int Table::_roundCounter = 1;

Table::Table(std::shared_ptr<Server> server) : _shoeDeck{ShoeDeck(consts::shoeDecks)} {
    _tableId = _roundCounter++;

    _isRoundContinues = false;
    _maxPlayers = consts::maxPlayersPerTable;
    _server = std::move(server);
    _roundNumber = 0;
    _wasRunned = false;
}

Table::Table(const Table &other)
{
    _shoeDeck = other._shoeDeck;

    _server = other._server;
    _players = other._players;
    _waitingPlayers = other._waitingPlayers;
    _croupier = other._croupier;
    _tableId = other._tableId;
    _roundNumber = other._roundNumber;
    _maxPlayers = other._maxPlayers;
    _isRoundContinues = other._isRoundContinues;
    _wasRunned = other._wasRunned;
}

void Table::addPlayer(Player p, int fd) {
    printLog("Adding player " + p.getName());
    //const std::lock_guard<std::mutex> lock(addWaitPlayerMutex);
    _players.insert({fd, p});
}

void Table::addWaitingPlayer(Player p, int fd) {
    printLog("Adding waiting player " + p.getName());
    //const std::lock_guard<std::mutex> lock(addPlayerMutex);
    _waitingPlayers.insert({fd, p});
}

bool Table::isRoundContinues() {
    //const std::lock_guard<std::mutex> lock(mutexRoundContinues);
    return _isRoundContinues;
}

bool Table::isTableEmpty() {
    //const std::lock_guard<std::mutex> lock(mutexTableEmpty);
    return _players.empty();
}

bool Table::isFreeSpace() {
    //const std::lock_guard<std::mutex> lock(mutexFreeSpace);
    return ((_players.size() + _waitingPlayers.size()) < _maxPlayers);
}

bool Table::wasRunned() {
    //const std::lock_guard<std::mutex> lock(mutexWasRunned);
    return _wasRunned;
}

void Table::throwAwayPreviousCards() {
    for (auto& pl : _players) {
        if (pl.second.getCardsAmount() > 0)
            pl.second.throwCards();
    }

    if (_croupier.getCardsAmount() > 0)
        _croupier.throwCards();
}

void Table::callDelay() {
    std::this_thread::sleep_for(std::chrono::milliseconds(consts::waitingSeconds * 750));
}

std::string Table::getPlayerResultAndSend(Player& pl, int fd) {
    std::stringstream ss;
    int wonPoints;

    if (pl.getCardsSum() <= 21) {
        if (_croupier.getCardsSum() <= 21) {
            if (pl.getCardsSum() > _croupier.getCardsSum()) {
                ss <<  "You win your bet x2: " << pl.getBet() << std::endl;
                pl.setBalance(pl.getBalance() + 2 * pl.getBet());

                wonPoints = pl.getBet() * 2;
            }
            else if (pl.getCardsSum() < _croupier.getCardsSum()) {
                ss << "You lost your bet: " << pl.getBet() << std::endl;
                // do nothing cause player already gave his bet to casino
                wonPoints = 0;
            }
            else {
                ss << "Tie with croupier. You keep your bet " << pl.getBet() << std::endl;
                pl.setBalance(pl.getBalance() + pl.getBet());
                wonPoints = pl.getBet();
            }
        }
        else {
            ss <<  "You win your bet x2: " << pl.getBet() << std::endl;
            pl.setBalance(pl.getBalance() + 2 * pl.getBet());
            wonPoints = pl.getBet() * 2;
        }
    }
    else {
        ss << "You lost your bet: " << pl.getBet() << std::endl;
        // do nothing cause player already gave his bet to casino
        wonPoints = 0;
    }

    _server->sendMessage(fd, std::to_string(wonPoints));

    return ss.str();
}


std::string Table::getCardsInfoOfAll(const std::pair<int, Player>& playerToSend) {
    int fd = playerToSend.first;
    // cards of all players first
    std::stringstream ss;

    ss << getCardsInfoOfOtherPlayers(playerToSend);

    // take info about cards of croupier
    ss << "croupier cards: " << _croupier.getStartRoundCardsStr() << " (X)" << std::endl;

    // take info about cards of certain player
    ss << "your cards: " << playerToSend.second.getCardsStr();
    ss << " (" << playerToSend.second.getCardsSum()<< ")" << std::endl;

    return ss.str();
}

std::string Table::getCardsInfoOfOtherPlayers(const std::pair<int, Player>& excPl) {
    int fd = excPl.first;

    // cards of other players
    std::stringstream ss;

    for (auto pl : _players) {
        // take info about other players' cards
        if (pl.first != excPl.first) {
            ss << pl.second.getName() << " cards: " << pl.second.getCardsStr();
            ss << " (" << pl.second.getCardsSum() << ")" << "  ";
        }
    }

    ss << std::endl;

    return ss.str();
}

void Table::setPlayersState() {
    for (auto& p : _players) {
        p.second.setPassState(false);
        p.second.setWinState(false);
    }
}

void Table::startRound() {
    // some init before starting round
    ++_roundNumber;
    throwAwayPreviousCards();
    setPlayersState();
    
    // shuffle the deck if few cards lefted (consts:perctang)
    if (_shoeDeck.isShuffleNeeded()) {
        _shoeDeck.shuffleShoe();
        printLog("Shuffle was made");
    }

    // log for server
    printLog(std::to_string(_players.size()) + " players in round #" + std::to_string(_roundNumber));

    // send start round info and make bet
    std::stringstream ss;

    for (auto pl : _players) {
        ss.str("");
        ss << "----------------ROUND #" << _roundNumber << "--------------" << std::endl;
        ss << "Your balance: " << pl.second.getBalance() << std::endl << std::endl;
        _server->sendMessage(pl.first, ss.str());
    }

    ss.str("");
    ss << "Enter your bet for round: ";
    for (auto pl : _players) {
        _server->getReply(pl.first);
        _server->sendMessage(pl.first, ss.str());
    }

    for (auto& pl : _players) {
        int fd = pl.first;

        int bet = stoi(_server->getReply(fd));

        printLog("player " + pl.second.getName() + " made bet " + std::to_string(bet));

        // and minusing balance by bet value and put it to bet
        pl.second.setBalance(pl.second.getBalance() - bet);
        pl.second.setBet(bet);
    }

    // now when all bets are done, giving everyone 2 cards
    for (auto& pl : _players) {
        Card card = _shoeDeck.getTopCard();
        pl.second.putCard(card);
        card = _shoeDeck.getTopCard();
        pl.second.putCard(card);
    }

    // giving croupier 2 cards
    Card card = _shoeDeck.getTopCard();
    _croupier.putCard(card);
    card = _shoeDeck.getTopCard();
    _croupier.putCard(card);


    // now we need to send card information to all clients
    for (auto pl : _players) {
        std::string cardsInfo = getCardsInfoOfAll(pl);

        // now send info about other players' cards, croupier and his(pl) cards to pl

        callDelay();
        _server->sendMessage(pl.first, cardsInfo);
    }

    // TODO: check for blackjack combination

    for (auto& pl : _players) {
        // means that starting player taking card or passing logic
        std::string yourTurn("Your turn!\n");
        std::string t1 = _server->getReply(pl.first); // this because 2 sends can't be byside

        callDelay();
        _server->sendMessage(pl.first, yourTurn); // <0> message TO player

        while(!pl.second.hasPassed()) {
            std::string askTakeOrPass = "\nDo you want to take card or pass? ";
            askTakeOrPass += "1. take   2. pass\n";

            std::string t2 = _server->getReply(pl.first); // this because 2 sends can't be byside

            _server->sendMessage(pl.first, askTakeOrPass); // <1> message TO player
            std::string playerChoice = _server->getReply(pl.first); // <2> message FROM player

            if (playerChoice == "1") {
                // take card
                Card card = _shoeDeck.getTopCard();
                pl.second.putCard(card);

                ss.str("");
                ss << std::endl << "you've taken " << card.getCardStr() << std::endl;
                ss << "your cards are: " << pl.second.getCardsStr() << " (";
                ss << pl.second.getCardsSum() << ")" <<  std::endl;

                if (pl.second.getCardsSum() > 21) {
                    ss << "sum of cards > 21 (" << pl.second.getCardsSum() << ")" << ". You lost";
                    ss << std::endl << std::endl;
                    pl.second.setPassState(true);
                }
                else if (pl.second.getCardsSum() == 21)
                {
                    ss << "sum of cards = 21" << ". You pass." << std::endl << std::endl;
                    pl.second.setPassState(true);
                }
                // sends: you've taken __ \n your cards are: __ \n sum of cards > 21 || sum of cards = 21 || nothing
            }
            else if (playerChoice == "2") {
                // pass
                pl.second.setPassState(true);
                ss.str("");
                ss << "you've passed" << std::endl << std::endl;
            }

            callDelay();
            _server->sendMessage(pl.first, ss.str()); // <3> message TO player

            // now send another message to player - he is passing or playing
            _server->getReply(pl.first); // this because 2 replies can't be byside
            if (pl.second.hasPassed())
                _server->sendMessage(pl.first, "pass"); // <4> message TO player
            else
                _server->sendMessage(pl.first, "play"); // <4> message TO player  
        }
    }

    // send all players info about other players cards and croupier
    for (auto pl : _players) {
        std::stringstream ss;
        ss << "Croupier shows his 2 cards: " << _croupier.getCardsStr();
        ss << " (" << _croupier.getCardsSum() << ")" << std::endl;

        std::string otherPlayersCards = getCardsInfoOfOtherPlayers(pl);

        ss << otherPlayersCards;

        // now send info about other players' cards, croupier and his(pl) cards to pl
        _server->getReply(pl.first); // this because 2 replies can't be byside

        callDelay();
        _server->sendMessage(pl.first, ss.str()); // <5> message to player
    }

    // now croupier takes cards until points are 17. If 17 or > - passes
    while(_croupier.getCardsSum() < 17) {
        Card card = _shoeDeck.getTopCard();
        _croupier.putCard(card);

        std::stringstream ss;
        ss << "Croupier takes card: " << card.getCardStr() << std::endl;

        // now send info about croupier card
        for (auto pl: _players) {
            _server->getReply(pl.first); // this because 2 replies can't be byside

            callDelay();
            _server->sendMessage(pl.first, ss.str()); // <6> message TO player
        }
    }

    for (auto pl : _players) {
        _server->getReply(pl.first); // this because 2 replies can't be byside

        std::stringstream ss;
        ss << "Croupier cards: ";

        callDelay();
        _server->sendMessage(pl.first, ss.str()); // <7> message TO player
        _server->getReply(pl.first);

        ss.str("");
        ss << _croupier.getCardsStr() << " (" << _croupier.getCardsSum() << ")" << std::endl;
        _server->sendMessage(pl.first, ss.str()); // <8> message TO player
    }

    for (auto pl : _players) {
        _server->getReply(pl.first);

        callDelay();
        _server->sendMessage(pl.first, "Croupier passes\n"); // <9> message TO player
    }

    for (auto pl : _players) {
        _server->getReply(pl.first); // this because 2 replies can't be byside
        std::string str = "\t\tYOUR RESULT\n";

        _server->sendMessage(pl.first, str); // <10> message TO player
    }

    for (auto& pl : _players) {
        _server->getReply(pl.first); // this because 2 replies can't be byside
        std::string result = getPlayerResultAndSend(pl.second, pl.first);

        _server->getReply(pl.first);

        callDelay();
        _server->sendMessage(pl.first, result); // <11> message TO player
    }

    for (const auto& p : _players) {
        _server->getReply(p.first); // temp
    }

    // in the end of round check if player disconnected
    for (auto pl = _players.cbegin(); pl != _players.cend();) {
        if ((*pl).second.getBalance() == 0) {
            _server->closeConnection((*pl).first);
            _players.erase(pl++);
        }
        else
            ++pl;
    }
}

void Table::printLog(const std::string& msg) {
    std::cout << "Table #" << _tableId << ": " << msg << std::endl;
}

void Table::run() {
    printLog("run()");
    _wasRunned = true;

    while(true) {
        // TODO: if player has no balance
        if (!this->isTableEmpty()) {
            // main logic of round here
            printLog("start of round");
            _isRoundContinues = true;
            startRound();
            _isRoundContinues = false;
            printLog("end of round");
        }
        
        // if there are waiting players - time to add them to actual players
        if (_waitingPlayers.size() > 0) {
            // we should add waiting players to table if they exist
            for (const auto& p:_waitingPlayers)
                printLog("Adding waiting player" + p.second.getName() + " to round");

            _players.insert(_waitingPlayers.begin(), _waitingPlayers.end());
            // and clean added waiting_players
            _waitingPlayers.clear();
        }

        // now let new players connect for delay time
        if (_players.size() < _maxPlayers) {
            if (_players.size() == 0) {
                while(_players.size()==0) {
                    _isRoundContinues = false;
                    printLog("waiting for new players...");
                    std::this_thread::sleep_for(std::chrono::milliseconds(consts::waitingSeconds * 3000));
                }
            }
            else {
                _isRoundContinues = false;
                printLog("waiting for new players...");
                std::this_thread::sleep_for(std::chrono::milliseconds(consts::waitingSeconds * 3000));
                _isRoundContinues = true;
            }
        }
    }
}

void Table::sendMsgToAllPlayers(std::string msg) {
    for (const auto& pair : _players) {
        _server->sendMessage(pair.first, msg);
    }
}