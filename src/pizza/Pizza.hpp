/*
** EPITECH PROJECT, 2020
** Pizza
** File description:
** Pizza
*/

#ifndef PIZZA_HPP_
#define PIZZA_HPP_

#include <string>
#include <map>
#include <vector>

#include "Food.hpp"

namespace Plazza {
    enum PizzaType {
        Regina = 1,
        Margarita = 2,
        Americana = 4,
        Fantasia = 8
    };
    enum PizzaSize {
        S = 1,
        M = 2,
        L = 4,
        XL = 8,
        XXL = 16
    };

    class Pizza {
        public:
            Pizza();
            ~Pizza();

            void setRecipe(std::string name);
            std::vector<Plazza::Food> getRecipe();

            bool cookPizza(std::vector<Plazza::Food>);

        protected:
        private:
    };
}

#endif /* !PIZZA_HPP_ */
