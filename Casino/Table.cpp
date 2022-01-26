#include "Table.hpp"

int Table::_counter = 1;
std::mutex addPlayerMutex;
std::mutex addWaitPlayerMutex;


Table::Table(Server* server) {
    _tableId = _counter++;

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
        startRound();
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
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            _isRoundContinues = true;
        }
    }
}

void Table::sendMsgToAllPlayers(std::string msg) {
    for (auto pair : _players) {
        _server->sendMessage(pair.first, msg.c_str());
    }
}