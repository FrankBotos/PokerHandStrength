#pragma once
#include <stdexcept>

class InvalidCardException : public std::runtime_error
{
public:
    InvalidCardException();
};

