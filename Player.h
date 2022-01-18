#include "Card.h"
#include <vector>
#include <string>

class Player {
public:
    bool AskCard();
    bool AskPass();
    bool AskBet();
private:
    std::vector<Card> _cards;
    std::string _name;
    bool _isPlaying;
    int _balance;
    int _currentBet;
};