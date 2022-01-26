#pragma once

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
    JACK = 10,
    QUEEN = 10,
    KING = 10,
    ACE = 11
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
    int GetValue();
private:
    CARD_RANK _rank;
    CARD_SUIT _suit;
};