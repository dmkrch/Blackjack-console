#include "Player.hpp"

Player::Player() {
    _hasPassed = false;
    _hasWon = false;
}

std::string Player::getCardsStr() {
    std::stringstream ss;
    for (auto card : _cards) {
        ss << card.getCardStr() << "  ";
    }

    return ss.str();
}

int Player::getCardsSum() const {
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