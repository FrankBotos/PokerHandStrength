#pragma once
#include "Card.hpp"
#include "Player.hpp"

class Board
{
private:
    std::vector<Card> deckMaster;    
public:
    Board();
    ~Board();
    
    void populateDeck();
    double evaluateHand(std::vector<Card> knownCards, int numOtherPlayers, int numSimulations);
    void simulateGame(int numMSims, std::vector<Card> knownCards, int numOtherPlayers, unsigned int& timesWon);
    Card takeRandomCardFromDeck(std::vector<Card>& deck);
    Card takeKnownCardFromDeck(std::vector<Card>& deck, Card cardToTake);
    int parseHand(std::vector<Card> hand);

};

