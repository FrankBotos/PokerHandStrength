#pragma once
#include "Card.hpp"

class Player
{
private:
    std::vector<Card> hand;
    int HSV = 0;
public:
    Player();
    ~Player();
    
    void giveCard(Card c);
    
    //getters
    Card getCard(int index);
    std::vector<Card> getHand();
    int getHSV();
    
    //setter
    void sortHand();
    void setHSV(int HSV);
    
};

