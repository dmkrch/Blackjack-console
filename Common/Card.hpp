#pragma once

#include <string>

enum class CARD_RANK {
    TWO = 2,
    THREE, 
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN, 
    JACK,
    QUEEN,
    KING,
    ACE
};

enum class CARD_SUIT {
    TRUMP,
    DIAMONDS,
    HEARTS,
    CLUBS,
    SPADES
};

class Card {
public:
    Card(CARD_RANK r, CARD_SUIT s);
    Card(CARD_RANK r);
    
    int getValue();
    CARD_RANK getCardRank() const { return _rank; }
    std::string getCardStr();
private:
    CARD_RANK _rank;
    CARD_SUIT _suit;
};