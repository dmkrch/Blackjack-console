enum class CARD_RANK {
    ONE = 1,
    TWO,
    THREE, 
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGH,
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
    int GetValue();
private:
    CARD_RANK _rank;
    CARD_SUIT _suit;
};