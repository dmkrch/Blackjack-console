#include "Card.hpp"

Card::Card(CARD_RANK r, CARD_SUIT s) : _rank{r}, _suit{s} {
}

Card::Card(CARD_RANK r) : _rank{r} {

}

int Card::getValue() const {
    switch(_rank) {
    case CARD_RANK::TWO:
    case CARD_RANK::THREE:
    case CARD_RANK::FOUR:
    case CARD_RANK::FIVE:
    case CARD_RANK::SIX:
    case CARD_RANK::SEVEN:
    case CARD_RANK::EIGHT:
    case CARD_RANK::NINE:
    case CARD_RANK::TEN:
        return static_cast<int>(_rank);
    case CARD_RANK::JACK:
    case CARD_RANK::QUEEN:
    case CARD_RANK::KING:
        return 10;
    case CARD_RANK::ACE:
        return 11;
    default:
        return -1;
    }
}

std::string Card::getCardStr() const {
    switch(_rank) {
    case CARD_RANK::TWO:
    case CARD_RANK::THREE:
    case CARD_RANK::FOUR:
    case CARD_RANK::FIVE:
    case CARD_RANK::SIX:
    case CARD_RANK::SEVEN:
    case CARD_RANK::EIGHT:
    case CARD_RANK::NINE:
    case CARD_RANK::TEN:
        return std::to_string(static_cast<int>(_rank));
    case CARD_RANK::JACK:
        return "JACK";
    case CARD_RANK::QUEEN:
        return "QUEEN";
    case CARD_RANK::KING:
        return "KING";
    case CARD_RANK::ACE:
        return "ACE";
    default:
        return "";
    }
}