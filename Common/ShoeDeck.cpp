#include "ShoeDeck.hpp"

void ShoeDeck::ShuffleShoe() {
    _shoeDeck.Shuffle();
}

bool ShoeDeck::isShuffleNeeded() {
    int amountOfFullCardsDeck = _amountOfDecksInShoe * 52;
    int leftCards = _shoeDeck.getCardsAmount();

    double leftCardsPercentage = leftCards / amountOfFullCardsDeck * 100;

    return leftCardsPercentage < consts::shufflePercantage;
}