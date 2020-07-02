/*
** EPITECH PROJECT, 2020
** Food
** File description:
** Food
*/

#include "Food.hpp"

Plazza::Food::Food(int foodQuantity)
: _quantity(foodQuantity)
{
}

Plazza::Food::Food(const Food &other)
{
    _name = other._name;
    _quantity = other._quantity;
}

Plazza::Food::~Food()
{
}

Plazza::Food &Plazza::Food::operator=(const Food &other)
{
    this->_name = other._name;
    this->_quantity = other._quantity;
    return *this;
}

bool Plazza::Food::take(unsigned short foodQuantity)
{
    if (_quantity - foodQuantity < 0)
        return false;
    _quantity -= foodQuantity;
    return true;
}

void Plazza::Food::refill(unsigned short int foodQuantity)
{
    _quantity += foodQuantity;
}

int Plazza::Food::getQuantity() const
{
    return _quantity;
}

std::string Plazza::Food::getName() const
{
    return _name;
}

void Plazza::Food::setName(std::string name)
{
    _name = name;
}
