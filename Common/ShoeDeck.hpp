#pragma once

#include "CardDeck.hpp"

class ShoeDeck {
public:
    ShoeDeck() {}
    ShoeDeck(int decks) {
        _amountOfDecksInShoe = decks;
        
        for (int i = 0; i < decks-1; ++i) {
            _shoeDeck = _shoeDeck + CardDeck();
        }
    }
    
    void ShuffleShoe();
    bool isShuffleNeeded();
    Card getTopCard() { return _shoeDeck.GetTopCard(); }
    int getAmountOfCards() const { return _shoeDeck.getCardsAmount(); }
private:
    int _amountOfDecksInShoe;
    CardDeck _shoeDeck;
};