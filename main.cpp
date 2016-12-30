//compiled in G++ 5.1.0
#include <iostream>
#include "Card.hpp"
#include "Board.hpp"

int main(){
    Board b;
    
    int numOtherPlayers = 4;
    int numSimulations = 10000;
    
    /**
     * 
     * CARD VALUES REFERENCE
     * 2-10 = regular numerical values
     * 11 = Jack
     * 12 = King
     * 13 = Queen
     * 14 = Ace
     * 
     * */
    
    //vector shold contain anywhere from 2 to 7 cards
    //first 2 are the player's cards, next 5 are the public cards (flop, river, turn)
    std::vector<Card> knownCards {Card(hearts, 14), Card(hearts, 9), Card(spades, 9)};
    
    double chanceOfWin = b.evaluateHand(knownCards, numOtherPlayers, numSimulations);
    
    std::cout << "Chance of win-> " << chanceOfWin << "% <-"; 
    
    return 0;
}