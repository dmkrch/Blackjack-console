#include "ShoeDeck.hpp"

void ShoeDeck::shuffleShoe() {
    _shoeDeck.shuffle();
}

bool ShoeDeck::isShuffleNeeded() const {
    int amountOfFullCardsDeck = _amountOfDecksInShoe * 52;
    int leftCards = _shoeDeck.getCardsAmount();

    double leftCardsPercentage = leftCards / amountOfFullCardsDeck * 100;

    return leftCardsPercentage < consts::shufflePercantage;
}