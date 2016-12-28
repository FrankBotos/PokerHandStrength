#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "InvalidCardException.hpp"

enum Suit{
    hearts,
    diamonds,
    spades,
    clubs
};

class Card{
private:
    size_t value;
    Suit suit;
    std::vector<std::string> suitsInStringFormat;
public:
    Card(Suit suite, size_t value);
    ~Card();
    
    //getters
    Suit getSuitIndex() const;
    size_t getValue() const;
    std::vector<std::string> getSuitVector() const;
    
    //simple ostream override
    friend std::ostream& operator<<(std::ostream& os, const Card& c);
    
    //equal override
    bool operator==(const Card& c);

};