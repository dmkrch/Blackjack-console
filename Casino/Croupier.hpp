#pragma once

#include "Card.hpp"

#include <vector>
#include <sstream>


class Croupier {
public:
    Croupier();
    void putCard(Card card) { _cards.push_back(card); }
    void throwCards() { _cards.clear(); }

    int getCardsAmount() const { return _cards.size(); }
    int getCardsSum();
    std::string getCardsStr();
    std::string getStartRoundCardsStr();

private:
    std::vector<Card> _cards;
};