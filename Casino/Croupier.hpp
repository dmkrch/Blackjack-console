#pragma once

#include "Card.hpp"

#include <vector>


class Croupier {
public:
    Croupier();
    void putCard(Card card) { _cards.push_back(card); }
    void throwCards() { _cards.clear(); }

    int getCardsAmount() const { return _cards.size(); }

private:
    std::vector<Card> _cards;
};