#include <vector>
#include <algorithm>
#include <random>

#include "Card.hpp"

class CardDeck {
public:
    CardDeck();
    void Shuffle();
    Card GetTopCard();
private:
    std::vector<Card> _cards;
};