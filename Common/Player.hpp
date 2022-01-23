#pragma once

#include "Card.hpp"
#include "Consts.hpp"
#include <vector>
#include <string>

class Player {
public:
    Player();
    bool AskCard();
    bool AskPass();
    bool AskBet();
    void setBalance(int b) { _balance = b; }
    void setName(std::string n) { _name = n; }
    std::string getName() const { return _name; }
private:
    std::vector<Card> _cards;
    std::string _name;
    bool _isPlaying;
    int _balance;
    int _currentBet;
};