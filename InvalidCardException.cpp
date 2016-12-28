#include "InvalidCardException.hpp"

InvalidCardException::InvalidCardException() : std::runtime_error("Error: Invalid card value. Card value must be between 2-14")
{
}