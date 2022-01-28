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


std::string Table::getCardsInfoOfAll(std::pair<int, Player> playerToSend) {
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

std::string Table::getCardsInfoOfOtherPlayers(std::pair<int, Player> excPl) {
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
        p.second.setLoseState(false);
        p.second.setWinState(false);
    }
}

void Table::startRound() {
    // some init before starting round
    ++_roundNumber;
    throwAwayPreviousCards();
    setPlayersState();

    // log for server
    printLog(std::to_string(_players.size()) + " players in round #" + std::to_string(_roundNumber));

    // send start round info and make bet
    std::stringstream ss;
    ss << "----------------ROUND #" << _roundNumber << "--------------" << std::endl << std::endl;
    ss << "Enter your bet for round: ";
    sendMsgToAllPlayers(ss.str());
    
    for (auto& pl : _players) {
        int fd = pl.first;
        int bet = stoi(_server->getReply(fd));
        std::cout.flush();

        // printLog use
        std::cout << "\tplayer " << pl.second.getName() << " made bet " << bet << std::endl;

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
        std::this_thread::sleep_for(std::chrono::milliseconds(consts::waitingSeconds * 1000));
        _server->sendMessage(pl.first, cardsInfo);
    }

    // TODO: check for blackjack combination

    
    // send message to other players to wait until he will do his turn
    for (auto player = ++_players.begin(); player != _players.end(); ++player) {
        std::string notifyOtherPl = "Wait until players make their turn...\n";
        _server->getReply((*player).first); // this because 2 sends can't be byside
        _server->sendMessage((*player).first, notifyOtherPl); // send message notifyOtherPl to 2nd, 3rd..
    }


    for (auto& pl : _players) {
        // means that starting player taking card or passing logic
        std::string yourTurn("Your turn!\n");
        _server->getReply(pl.first); // this because 2 sends can't be byside
        _server->sendMessage(pl.first, yourTurn); // <0> message TO player

        std::this_thread::sleep_for(std::chrono::milliseconds(consts::waitingSeconds * 1000));

        while(!pl.second.hasPassed()) {
            std::string askTakeOrPass = "\nDo you want to take card or pass?";
            askTakeOrPass += "\n";
            askTakeOrPass += "1. take   2. pass  ";
            _server->getReply(pl.first); // this because 2 sends can't be byside
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
                    ss << "sum of cards > 21 (" << pl.second.getCardsSum() << ")" << ". You lost" << std::endl;
                    pl.second.setPassState(true);
                    pl.second.setLoseState(true);
                }
                else if (pl.second.getCardsSum() == 21)
                {
                    ss << "sum of cards = 21" << ". You pass." << std::endl;
                    pl.second.setPassState(true);
                }
                // sends: you've taken __ \n your cards are: __ \n sum of cards > 21 || sum of cards = 21 || nothing
            }
            else if (playerChoice == "2") {
                // pass
                pl.second.setPassState(true);
                ss.str("");
                ss << "you've passed" << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(consts::waitingSeconds * 1000));
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
            std::this_thread::sleep_for(std::chrono::milliseconds(consts::waitingSeconds * 2000));
            _server->sendMessage(pl.first, ss.str()); // <6> message TO player
        }
    }

    for (auto pl : _players) {
        _server->getReply(pl.first); // this because 2 replies can't be byside

        std::stringstream ss;
        ss << "Croupier cards: ";

        std::this_thread::sleep_for(std::chrono::milliseconds(consts::waitingSeconds * 1500));
        _server->sendMessage(pl.first, ss.str()); // <7> message TO player
        _server->getReply(pl.first);

        ss.str("");
        ss << _croupier.getCardsStr() << " (" << _croupier.getCardsSum() << ")" << std::endl;
        _server->sendMessage(pl.first, ss.str()); // <8> message TO player
    }



    for (auto pl : _players) {
        _server->getReply(pl.first); // this because 2 replies can't be byside
        std::string str = "RESULTS:\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(consts::waitingSeconds * 1000));
        _server->sendMessage(pl.first, str); // <9> message TO player
    }

    // and now we need to calculate croupier sum of cards, players sum and print them results
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