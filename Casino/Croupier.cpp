#include "Croupier.hpp"

Croupier::Croupier() {
    
}

std::string Croupier::getStartRoundCardsStr() {
    std::stringstream ss;
    if (_cards.size() > 0)
        ss << _cards[0].getCardStr() << "  X ";

    return ss.str();
}

std::string Croupier::getCardsStr() {
    std::stringstream ss;
    for (auto card : _cards) {
        ss << card.getCardStr() << "  ";
    }

    return ss.str();
}


int Croupier::getCardsSum() {
    int sum = 0;
    int amountOfAces = 0;

    // first get the sum without aces
    for (auto card : _cards) {
        int cardVal = card.getValue();

        if (cardVal != 11)
            sum += cardVal;
        else 
            ++amountOfAces;
    }

    // then adding aces one by one, so that sum will not exceed 21 points
    while(amountOfAces-- > 0) {
        if (sum + 11 <= 21)
            sum += 11;
        else 
            break;
    }

    return sum;
}