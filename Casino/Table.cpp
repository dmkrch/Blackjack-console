#include "Table.hpp"

Table::Table(Server* server) {
    _tableId = _counter++;

    _isRoundContinues = false;
    _maxPlayers = consts::maxPlayersPerTable;
    _server = server;
    _roundNumber = 0;
}

void Table::AddPlayer(Player p, int fd) {
    printLog("Adding player " + p.getName());
    _players.insert({fd, p});
}

void Table::AddWaitingPlayer(Player p, int fd) {
    printLog("Adding waiting player " + p.getName());
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


    sendMsgToAllPlayers("Welcome to round #" + std::to_string(_roundNumber));
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void Table::printLog(std::string msg) {
    std::cout << "Table #" << _tableId << ": " << msg << std::endl;
}

void Table::run() {
    printLog("run()");
    while(_players.size() > 0) {
        // main logic of round here
        printLog("start of round");
        startRound();
        printLog("end of round");

        // we should add waiting players to table if they exist
        for (auto p:_waitingPlayers)
            printLog("Adding waiting player" + p.second.getName() + " to round");
        

        _players.insert(_waitingPlayers.begin(), _waitingPlayers.end());
        // and clean added waiting_players
        _waitingPlayers.clear();

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

int Table::_counter = 1;