#pragma once

#include "Card.hpp"
#include "Consts.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <numeric>

class Player {
public:
    Player();
    void setBalance(int b) { _balance = b; }
    void setName(std::string n) { _name = n; }
    void setBet(int bet) { _currentBet = bet; }
    void putCard(Card card) { _cards.push_back(card); }
    void throwCards() { _cards.clear(); }
    void setPassState(bool state) { _hasPassed = state; }
    void setWinState(bool state) { _hasWon = state; }
    void setLoseState(bool state) { _hasLost = state; }
    
    std::string getName() const { return _name; }
    int getBet() const { return _currentBet; }
    int getBalance() const { return _balance; }
    int getCardsAmount() const { return _cards.size(); }
    int getCardsSum() const;
    bool isPlayingRoundState() const { return _isPlaying; }
    bool hasPassed() const { return _hasPassed; }
    bool hasWon() const { return _hasWon; }
    bool hasLost() const { return _hasLost; }
    void setRoundPlayingState(bool state) { _isPlaying = state; }
    std::string getCardsStr();
private:
    std::vector<Card> _cards;
    std::string _name;
    bool _isPlaying;
    bool _hasPassed;
    bool _hasWon;
    bool _hasLost;
    int _balance;
    int _currentBet;
};