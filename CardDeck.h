#include <vector>
#include "Card.h"

class CardDeck {
public:
    CardDeck();
    void Shuffle();
    Card GetTopCard();
private:
    std::vector<Card> _cards;
};