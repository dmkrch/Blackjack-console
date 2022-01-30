#include "Croupier.hpp"

Croupier::Croupier() {
    
}

std::string Croupier::getStartRoundCardsStr() const {
    std::stringstream ss;
    if (!_cards.empty())
        ss << _cards[0].getCardStr() << "  X ";

    return ss.str();
}

std::string Croupier::getCardsStr() const {
    std::stringstream ss;
    for (const auto& card : _cards) {
        ss << card.getCardStr() << "  ";
    }

    return ss.str();
}


int Croupier::getCardsSum() const {
    int sum = 0;
    int amountOfAces = 0;

    // first get the sum without aces
    for (const auto& card : _cards) {
        int cardVal = card.getValue();

        if (cardVal != 11)
            sum += cardVal;
        else 
            ++amountOfAces;
    }

    // then adding aces one by one
    while(amountOfAces-- > 0) {
        if (sum + 11 <= 21)
            sum += 11;
        else {
            sum += 1;
        }
    }

    return sum;
}