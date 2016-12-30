#include "Player.hpp"
#include <algorithm>

Player::Player()
{
}

Player::~Player()
{
}

void Player::giveCard(Card c){
    hand.push_back(c);
}

//getters
Card Player::getCard(int index){
    return hand.at(index);
}

int Player::getHSV(){
    return HSV;
}

std::vector<Card>& Player::getHand(){
    return hand;
}

//setters
void Player::sortHand(){
    std::sort(hand.begin(), hand.end(), [](const Card& c1, const Card& c2) -> bool{
        return c1.getValue() > c2.getValue();
    });
}

void Player::setHSV(int HSV){
    this->HSV = HSV;
}
