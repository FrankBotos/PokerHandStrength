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
    double evaluateHand(std::vector<Card>& knownCards, int numOtherPlayers, int numSimulations);
    void simulateGame(int numMSims, std::vector<Card> knownCards, int numOtherPlayers, unsigned int& timesWon);//passing the vector by value instead of reference because we use the value to launch our threads (using a reference here stops program from compililng)
    Card takeRandomCardFromDeck(std::vector<Card>& deck);
    Card takeKnownCardFromDeck(std::vector<Card>& deck, Card& cardToTake);
    int parseHand(std::vector<Card>& hand);

};

