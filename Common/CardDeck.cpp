#include "CardDeck.hpp"

auto rng = std::default_random_engine {};

// accepts int - amount of decks that should be in one deck
CardDeck::CardDeck() {
    std::vector<CARD_RANK> ranks;
    ranks.push_back(CARD_RANK::TWO); // 0 index
    ranks.push_back(CARD_RANK::THREE);
    ranks.push_back(CARD_RANK::FOUR);
    ranks.push_back(CARD_RANK::FIVE);
    ranks.push_back(CARD_RANK::SIX);
    ranks.push_back(CARD_RANK::SEVEN);
    ranks.push_back(CARD_RANK::EIGHT);
    ranks.push_back(CARD_RANK::NINE);
    ranks.push_back(CARD_RANK::TEN);
    ranks.push_back(CARD_RANK::JACK);
    ranks.push_back(CARD_RANK::QUEEN);
    ranks.push_back(CARD_RANK::KING);
    ranks.push_back(CARD_RANK::ACE); // 12 index

    // that cycle puts 52 cards: 4 twos, 4 threes, ... 4 aces
    for (int i = 0; i < 13; ++i) {
        CARD_RANK r = ranks[i];
        // in r we have ranks that me need to add to deck

        // so we add 4 cards of same rank to deck
        // TODO: add card_suit as well, but no need in console application
        for (int i = 0; i < 4; ++i) {
            _cards.push_back(Card(r));
        }
    }

    // and now shuffle the deck 
    shuffle();
}

void CardDeck::shuffle() {
    std::shuffle(std::begin(_cards), std::end(_cards), rng);
}

Card CardDeck::getTopCard() {
    Card card = _cards.back();
    _cards.pop_back();
    return card;
}

// just inserts cards of deck2 into this->deck
CardDeck CardDeck::operator+(const CardDeck& deck2) {
    this->_cards.insert(this->_cards.end(), deck2._cards.begin(), deck2._cards.end());
    return *this;
}