#pragma once

#include <vector>
#include <algorithm>
#include <random>

#include "Card.hpp"

class CardDeck {
public:
    CardDeck();
    void Shuffle();
    Card GetTopCard();
    int getCardsAmount() const { return _cards.size(); }

    CardDeck operator+(const CardDeck& deck2);
private:
    std::vector<Card> _cards;
};