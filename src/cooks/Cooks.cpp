/*
** EPITECH PROJECT, 2020
** cooks
** File description:
** Cooks
*/


#include "Cooks.hpp"

#include "Kitchen.hpp"

Plazza::Cooks::Cooks(int pizza, std::map<Plazza::Cooks::PizzaType, std::map<std::string, int>> recipe, unsigned short mult)
: _pizza(pizza), _recipe(recipe), _multiplier(mult)
{
}

Plazza::Cooks::~Cooks()
{
}

std::tuple<std::string, uint16_t> Plazza::Cooks::getIngredients(const std::string &ingredient, ingredient_t &recipe) const
{
    ingredient_t::iterator it = recipe.find(ingredient);
    return (std::make_tuple(it->first, it->second));
}

 bool Plazza::Cooks::getAllIngredients(PizzaType pizza, Plazza::Kitchen *kitchen)
{
    auto it = _recipe.find(pizza);
    auto resPizz = it->second;
    int res = 0;
    int pizzSize = resPizz.size();

    for(auto it = resPizz.begin(); it != resPizz.end(); ++it) {
        if (kitchen->takeIngredient(it->first, it->second) == true)
            ++res;
        if (res == pizzSize)
            return (true);
    }
    return (true);
}

uint16_t Plazza::Cooks::PizzaTime(PizzaType pizza) const
{
    for (int i = 0; i != SLEN(pizzaMap); i++) {
        if (pizza == pizzaMap[i].pizzaType) {
            return (pizzaMap[i].time * _multiplier);
        }
    }
    return (0);
}

Plazza::Cooks::PizzaType Plazza::Cooks::getLastPizza(Plazza::Kitchen *kitchen)
{
    Plazza::Cooks::PizzaType pizza = Regina;
    BMutex mute;

    usleep(1000);
    mute.lock();
    pizza = static_cast<Plazza::Cooks::PizzaType> (kitchen->takePizza());
    mute.unlock();
    return (pizza);
}

void *Plazza::Cooks::cookLoop(void *arg)
{
    Plazza::Kitchen *kitchen = static_cast<Plazza::Kitchen *>(arg);
    Plazza::Cooks::PizzaType pizza = getLastPizza(kitchen);

    if (pizza != NONE) {
        auto it = _recipe.find(pizza);
        if (getAllIngredients(it->first, kitchen) == true) {
            std::cout << "Started cooking !" << std::endl;
            sleep(PizzaTime(pizza));
            std::cout << "Finished cooking !" << std::endl;
            kitchen->getClient()->sendResponse(createMessage(Plazza::messageType::FINISH_PIZZA, (char*)"", getpid(), 0));
        }
    } else {
        std::cout << "No pizza" << std::endl;
        return nullptr;
    }
    return nullptr; 
}