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
    bool AskCard();
    bool AskPass();
    bool AskBet();
    void setBalance(int b) { _balance = b; }
    void setName(std::string n) { _name = n; }
    void setBet(int bet) { _currentBet = bet; }
    void putCard(Card card) { _cards.push_back(card); }
    void throwCards() { _cards.clear(); }
    
    std::string getName() const { return _name; }
    int getBet() const { return _currentBet; }
    int getBalance() const { return _balance; }
    int getCardsAmount() const { return _cards.size(); }
    int getCardsSum();
    std::string getCardsStr();
private:
    std::vector<Card> _cards;
    std::string _name;
    bool _isPlaying;
    int _balance;
    int _currentBet;
};