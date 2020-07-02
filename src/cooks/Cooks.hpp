/*
** EPITECH PROJECT, 2020
** Cooks
** File description:
** Cooks
*/

#pragma once

#include <map>
#include <string>
#include <tuple>
#include <ctime>

typedef std::map<std::string, uint16_t>ingredient_t;

#define SLEN(a) (sizeof(a) / sizeof(*a))

namespace Plazza {
    class Kitchen;
};

namespace Plazza {
    class Cooks {
        public :
            enum PizzaType {
                NONE = 0,
                Regina = 1,
                Margarita = 2,
                Americana = 4,
                Fantasia = 8
            };

        typedef std::map<Plazza::Cooks::PizzaType, std::map<std::string, int>> recipe_t;

        public:
            Cooks(int, std::map<Plazza::Cooks::PizzaType, std::map<std::string, int>>, unsigned short);
            ~Cooks();

            void *cookLoop(void *);
            std::tuple<std::string, uint16_t> getIngredients(const std::string &, ingredient_t &) const;
            bool getAllIngredients(PizzaType pizza, Plazza::Kitchen *kitchen);
            Plazza::Cooks::PizzaType getLastPizza(Plazza::Kitchen *kitchen);
            uint16_t PizzaTime(PizzaType) const;

        protected:
        private:
            int _pizza;
            recipe_t &_recipe;
            uint16_t _multiplier;
    };
}

typedef struct pizzaName_s {
    Plazza::Cooks::PizzaType pizzaType;
    uint16_t time;
} pizzaName_t;

static const pizzaName_t pizzaMap[] = {
    {Plazza::Cooks::Regina, 2},
    {Plazza::Cooks::Margarita, 1},
    {Plazza::Cooks::Americana, 2},
    {Plazza::Cooks::Fantasia, 4}
};
