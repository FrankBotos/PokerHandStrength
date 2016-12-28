#include "Card.hpp"

Card::Card(Suit suit, size_t value){
    this->suit = suit;
    this->value = value;
    
    //initializing our array of suits in string format for easy retreival at a later stage
    this->suitsInStringFormat = { "hearts", "diamonds", "spades", "clubs" };
    
    //quick validity check
    if(value < 2 || value > 14){
        throw InvalidCardException();
    }
    
    
}
Card::~Card(){}

//getters
Suit Card::getSuitIndex() const{
    return this->suit;
}

size_t Card::getValue() const{
    return this->value;
}

std::vector<std::string> Card::getSuitVector() const {
    return this->suitsInStringFormat;
}

bool Card::operator==(const Card& c){
    bool e = false;
    
    if((c.getSuitIndex() == this->getSuitIndex()) && (c.getValue() == this->getValue())){
        e = true;
    }
    
    return e;
}


std::ostream& operator<<(std::ostream& os, const Card& c){
    //retrieve our rank
    if (c.getValue() < 11){
        os << c.getValue();
    } else {
        if (c.getValue() == 11){
            os << "Jack";
        } else if (c.getValue() == 12){
            os << "King";
        } else if (c.getValue() == 13){
            os << "Queen";
        } else {
            os << "Ace";
        }
    }
    
    //retrieve our suit
    os << " of " << c.getSuitVector().at(c.getSuitIndex());
    
    return os;
}