#include "Board.hpp"
#include <thread>
#include <random>
#include <time.h>

Board::Board()
{
    populateDeck();  
}

Board::~Board()
{
}

void Board::populateDeck(){
    deckMaster.clear();
    for (int i = 2; i <= 14; i++){
        deckMaster.push_back(Card(hearts, i));
        deckMaster.push_back(Card(diamonds, i));
        deckMaster.push_back(Card(spades, i));
        deckMaster.push_back(Card(clubs, i));
    }
}

double Board::evaluateHand(std::vector<Card>& knownCards, int numOtherPlayers, int numSimulations){    
    
    numOtherPlayers++;//increment by one since index 0 is the hand we are evaluating
    
    auto numThreads = std::thread::hardware_concurrency();
    
    
    int numSimsPerThread = numSimulations/numThreads;
    bool lastThreadDifferent = false;
    if(numSimulations%numThreads != 0){
        lastThreadDifferent = true;
    }
    
    //apparently this is how you launch a thread in a c++ CLASS
    //auto t1 = std::thread(&Board::simulateGame, this, 5);
    //t1.join();
    
    std::vector<unsigned int> numWins (numThreads, 0);
    
    std::vector<std::thread> threads(numThreads);
    
    
    
    for(unsigned int i = 0; i < numThreads; i++){//launching our threads
        if (i == numThreads-1 && lastThreadDifferent){//making certain our number of sims are equally divided among all threads
            numSimsPerThread = numSimsPerThread + (numSimulations%numThreads);
        }
        threads.at(i) = std::thread(&Board::simulateGame, this, numSimsPerThread, knownCards, numOtherPlayers, std::ref(numWins.at(i)));//nte using the reference wrapper allows us to send the value as a reference, and change it without a return value
    }
    
    for(unsigned int i = 0; i < numThreads; i++){//joining our threads
        threads.at(i).join();
    }
    
    double finalValue = 0;
    for(unsigned int i = 0; i < numWins.size(); i++){
        finalValue += numWins.at(i);
    }
    
    return (finalValue/numSimulations) * 100;
    
}

void Board::simulateGame(int numSims, std::vector<Card> knownCards, int numOtherPlayers, unsigned int& timesWon){
    
    for(int i = 0; i < numSims; i++){
        
        std::vector<Card> localDeck = deckMaster;
        std::vector<Card> publicCards;//flop, turn river
        std::vector<Player> players(numOtherPlayers, Player());//vector to hold our players - index 0 is the player whose hand we are evaluating
        
        /**
         * Algorithm:
         * 1. deal cards to players
         * */
         
        //1. deal known all of our known cards
        //first 2 cards will always be our main player's cards
        for(int i = 0; i < 2; i ++){
            players.at(0).giveCard(takeKnownCardFromDeck(localDeck, knownCards.at(i)));
        }
        
        //if we have public cards, add to public card vector
        //NOTE: public card count must not exceed 5!
        if (knownCards.size() > 2) {
            for(unsigned int i = 2; i < knownCards.size(); i++){
                publicCards.push_back(takeKnownCardFromDeck(localDeck, knownCards.at(i)));
            }
        }
        
        //if there are any unknown public cards, we randomly deal them out here
        if(publicCards.size() >= 0 && (publicCards.size() < 6)){
            for(unsigned int i = 0; i < (publicCards.size() - 5); i ++){
                publicCards.push_back(takeRandomCardFromDeck(localDeck));
            }
        }
        
        //now we deal random cards to our other players
        for(unsigned int i = 1; i < players.size(); i++){
            for(int j = 0; j < 2; j++){
                players.at(i).giveCard(takeRandomCardFromDeck(localDeck));
            }
        }
        
        //now we fill out our player's hands by combining their private cards with their public cards
        //this wil make later sorting easier
        for(unsigned int i = 0; i < players.size(); i++){
            for(unsigned int j = 0; j < publicCards.size(); j++){
                players.at(i).giveCard(publicCards.at(j));
            }
        }
        
        //now we sort each player's hand from highest to lowest valued cards
        //this should make finding the best possible hand of 5 easier
        for(unsigned int i = 0; i < players.size(); i++){
            players.at(i).sortHand();
        }
        
        //finally, we find the absolute best possible hand of 5 for each player, and provide a "hand strength value"
        //whichever hand has the highest HSV wins this round
        //if our main player wins this round, we increment numWins by 1
        for(unsigned int i = 0; i < players.size(); i++){
            players.at(i).setHSV(parseHand(players.at(i).getHand()));
        }
        
        //now we just compare the values and increment numWins by 1 if our player won this round
        int playerWon = true;
        int playerHSV = players.at(0).getHSV();
        for(unsigned int i = 1; i < players.size(); i++){
            if(players.at(i).getHSV() > playerHSV){
                playerWon = false;
                break;
            }
        }
        if(playerWon){
            //in case of debug
            //std::cout << "WON\n";            
            timesWon++;
        }
        
//        //output our deck for debug purposes
//        std::cout << "Player (";
//        for(unsigned int i = 0; i < players.at(0).getHand().size(); i++){
//            if(i == 2){
//                std::cout << " ->";
//            }
//            
//            std::cout << players.at(0).getCard(i) << ", ";
//            
//            if(i == players.at(0).getHand().size()-1){
//                std::cout << "<-)" << std::endl;
//            }
//            
//        }
//        
//        for(unsigned int i = 1; i < players.size(); i++){
//            std::cout << "Opponent " << i << " (";
//            for(unsigned int j = 0; j < players.at(i).getHand().size(); j++){
//                if(j == 2){
//                    std::cout << " ->";
//                }
//                std::cout << players.at(i).getCard(j);
//                std::cout << ", ";
//                if (j == players.at(i).getHand().size()-1){
//                    std::cout << "<-";
//                }
//            }
//            std::cout << ")" << std::endl;
//        }
//        
//        std::cout << std::endl << "Public Cards - > ";
//        for(unsigned int i = 0; i < publicCards.size(); i++){
//            std::cout << publicCards.at(i) << ", " ;
//        }
//        std::cout << std::endl << std::endl << "Deck size -> " << localDeck.size() << " - Cards: " << std::endl << "--------------------" << std::endl;
//        for(unsigned int i = 0; i < localDeck.size(); i++){
//            std::cout << localDeck.at(i) << std::endl;
//        }
        
         
    }

}

Card Board::takeRandomCardFromDeck(std::vector<Card>& deck){
    Card cardToTake(spades, 14);//placeholder card, is updated after
    
    //generate a random int to decide which card we draw
    static thread_local std::mt19937* generator = nullptr;
    if(!generator) generator = new std::mt19937(clock() + std::hash<std::thread::id>()(std::this_thread::get_id()));
    std::uniform_int_distribution<int> distribution(1,deck.size()-1);
    int randomInt = distribution(*generator);
    
    
    //we copy the card to our own variable
    cardToTake = deck.at(randomInt);
    
    //we delete the card we took from the deck
    deck.erase(deck.begin() + randomInt);
    
    return cardToTake;
    
}

Card Board::takeKnownCardFromDeck(std::vector<Card>& deck, Card& cardToTake){
    
    for(unsigned int i = 0; i < deck.size(); i++){//cycle through our vector and delete the card once it has been found
        if(deck.at(i) == cardToTake){
            deck.erase(deck.begin() + i);
            break;
        }
    }
    
    return cardToTake;//we simply return the card we know has been taken to maintain our system of giving cards to Player class
}

//this function takes a vector of 7 cards and finds the best possible hand + returns hand strength value
int Board::parseHand(std::vector<Card>& hand){
    
    //find if royal flush
    bool valuesMatch = true;
    std::vector<Card> royalFlushPreset { Card(spades, 14), Card(spades, 13), Card(spades, 12), Card(spades, 11), Card(spades, 10) };
    for(int i = 0; i < 5; i++){
        if(hand.at(i).getValue() != royalFlushPreset.at(i).getValue()){
            valuesMatch = false;
            break;
        }
    }
    if(valuesMatch){
        bool suitsMatch = true;
        Suit refSuit = hand.at(0).getSuitIndex();
        for(int i = 1; i < 5; i++){
            if(hand.at(i).getSuitIndex() != refSuit){
                suitsMatch = false;
                break;
            }
        }
        
        if(suitsMatch){//if this executes it means we have a royal flush! HSV is 1000
            return 1000;
        }
        
    }
    
    //find if straight flush (these values can be used later for regular flush)
    std::vector<Card> spades;
    std::vector<Card> clubs;
    std::vector<Card> hearts;
    std::vector<Card> diamonds;
    for(unsigned int i = 0; i < hand.size(); i++){
        if(hand.at(i).getSuitIndex() == 2){
            spades.push_back(hand.at(i));
        } else if (hand.at(i).getSuitIndex() == 3){
            clubs.push_back(hand.at(i));
        } else if (hand.at(i).getSuitIndex() == 0){
            hearts.push_back(hand.at(i));
        } else {
            diamonds.push_back(hand.at(i));
        }
    }
    
    if(hearts.size() >= 5){
        bool straightFlush = true;
        for(unsigned int i = 0; i < hearts.size()-1; i++){
            if (!straightFlush){
                break;
            }
            if(hearts.at(i).getValue() != hearts.at(i+1).getValue()+1){
                straightFlush = false;
            }
        }
        if(straightFlush){
            return 900 + hearts.at(0).getValue();
        }
    }
    
    if(diamonds.size() >= 5){
        bool straightFlush = true;
        for(unsigned int i = 0; i < diamonds.size()-1; i++){
            if (!straightFlush){
                break;
            }
            if(diamonds.at(i).getValue() != diamonds.at(i+1).getValue()+1){
                straightFlush = false;
            }
        }
        if(straightFlush){
            return 900 + diamonds.at(0).getValue();
        }
    }
    
    if(spades.size() >= 5){
        bool straightFlush = true;
        for(unsigned int i = 0; i < spades.size()-1; i++){
            if (!straightFlush){
                break;
            }
            if(spades.at(i).getValue() != spades.at(i+1).getValue()+1){
                straightFlush = false;
            }
        }
        if(straightFlush){
            return 900 + spades.at(0).getValue();
        }
    }
    
    if(clubs.size() >= 5){
        bool straightFlush = true;
        for(unsigned int i = 0; i < clubs.size()-1; i++){
            if (!straightFlush){
                break;
            }
            if(clubs.at(i).getValue() != clubs.at(i+1).getValue()+1){
                straightFlush = false;
            }
        }
        if(straightFlush){
            return 900 + clubs.at(0).getValue();
        }
    }
    
    //find if 4 of a kind
    std::vector<int> values { 0,0,0,0,0,0,0,0,0,0,0,0,0 };//this vector contains useful info, can be used to determine other hands!
    
    for(unsigned int i = 0; i < hand.size(); i++){
        values.at(hand.at(i).getValue() - 2)++;
    }
    
    for(unsigned int i = 0; i < values.size(); i++){
        if(values.at(i) == 4){
            return 800 + i + 2;
        }
    }
    
    //find if full house
    bool triple = false;
    bool two = false;
    int val1;
    int val2;
    for(unsigned int i = 0; i < values.size(); i++){
        if (values.at(i) == 3){
            triple = true;
            val1 = i + 2;
        }
        if(values.at(i) == 2){
            two = true;
            val2 = i + 2;
        }
    }
    
    if(triple && two){
        return 700 + val1 + val2;
    }
    
    //find if regular flush
    int highCard = 0;
    if(spades.size() == 5){
        for(int i = 0; i < 5; i++){
            highCard += spades.at(i).getValue();
        }
        return 600 + highCard;
    }
    if(clubs.size() == 5){
        for(int i = 0; i < 5; i++){
            highCard += clubs.at(i).getValue();
        }
        return 600 + highCard;
    }
    if(hearts.size() == 5){
        for(int i = 0; i < 5; i++){
            highCard += hearts.at(i).getValue();
        }
        return 600 + highCard;
    }
    if(diamonds.size() == 5){
        for(int i = 0; i < 5; i++){
            highCard += diamonds.at(i).getValue();
        }
        return 600 + highCard;
    }
    
    //find if regular straight
    int count = 0;
    for(unsigned int i = 0; i < hand.size()-1; i++){
        if(count == 4){
            highCard = hand.at(i-4).getValue();
            break;
        }
        if(hand.at(i).getValue() == hand.at(i+1).getValue()+1){
            count++;
        } 
        else if (hand.at(i).getValue() == hand.at(i+1).getValue()){
            //if they are equal we don't do anything
            //this accounts for duplicates in an easy/efficient way
        }
        else {
            count = 0;
        }
    }
    
    if(count == 4){
        return 500 + highCard;
    }
    
    //find if 3 of a kind
    for(unsigned int i = 0; i < values.size(); i++){
        if(values.at(i) == 3){
            return 400 + i + 2;
        }
    }
    
    //find if 2 pairs
    count = 0;
    highCard = 0;
    for(unsigned int i = 0; i < values.size(); i++){
        if (count == 2){
            break;
        }
        if(values.at(i) == 2){
            highCard += i + 2;
            count++;
        }
    }
    if(count == 2) {
        return 300 + highCard;
    }
    
    //find if single pair
    for(unsigned int i = 0; i < values.size(); i++){
        if(values.at(i) == 2){
            return 200 + i + 2;
        }
    }
    
    //if we did not find any sort of pairing or hand, we simply return value of high cards
    highCard = 100;
    for(int i = 5; i < 5; i++){
        highCard += hand.at(i).getValue();
    }
    return highCard;
    
}