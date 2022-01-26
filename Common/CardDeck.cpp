#include "CardDeck.hpp"

auto rng = std::default_random_engine {};

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
    Shuffle();
}

void CardDeck::Shuffle() {
    std::shuffle(std::begin(_cards), std::end(_cards), rng);
}

Card CardDeck::GetTopCard() {
    Card card = _cards.back();
    _cards.pop_back();
    return card;
}