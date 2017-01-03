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
    
    //since we are simulating our games, most of our variables are not class level, and thereefore we can use the const modifier for most of these functions
    //as we are not altering any class level variables
    void populateDeck();
    double evaluateHand(std::vector<Card>& knownCards, int numOtherPlayers, int numSimulations) const;
    void simulateGame(int numMSims, std::vector<Card> knownCards, int numOtherPlayers, unsigned int& timesWon) const;//passing the vector by value instead of reference because we use the value to launch our threads (using a reference here stops program from compililng)
    Card takeRandomCardFromDeck(std::vector<Card>& deck) const;
    Card takeKnownCardFromDeck(std::vector<Card>& deck, Card& cardToTake) const;
    int parseHand(std::vector<Card>& hand) const;

};

