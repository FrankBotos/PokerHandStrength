//compiled in GCC 5.1.0
#include <iostream>
#include "Card.hpp"
#include "Board.hpp"

int main(){
    Board b;
    
    int numOtherPlayers = 2;
    int numSimulations = 10000;
    
    
    //vector shold contain anywhere from 2 to 7 cards
    //first 2 are the player's cards, next 5 are the public cards (flop, river, turn)
    std::vector<Card> knownCards {Card(hearts, 14), Card(hearts, 9)};
    
    double chanceOfWin = b.evaluateHand(knownCards, numOtherPlayers, numSimulations);
    
    return 0;
}