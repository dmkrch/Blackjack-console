#include "Player.hpp"

Player::Player() {

}

bool Player::AskCard() {
    return true;
}

bool Player::AskPass() {
    return true;
}

bool Player::AskBet() {
    return true;
}

std::string Player::getCardsStr() {
    std::stringstream ss;
    for (auto card : _cards) {
        ss << card.getCardStr() << "  ";
    }

    return ss.str();
}

int Player::getCardsSum() {
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