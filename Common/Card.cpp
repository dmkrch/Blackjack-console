#include "Card.hpp"

Card::Card(CARD_RANK r, CARD_SUIT s) : _rank{r}, _suit{s} {
}

Card::Card(CARD_RANK r) : _rank{r} {

}

int Card::GetValue() {
    return static_cast<int>(_rank);
}