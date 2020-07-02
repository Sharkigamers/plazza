/*
** EPITECH PROJECT, 2020
** Food
** File description:
** Food
*/

#ifndef FOOD_HPP_
#define FOOD_HPP_

#include "IFood.hpp"

namespace Plazza {
    class Food : public Plazza::IFood {
        public:
            Food(int foodQuantity = 5);
            Food(const Food &other);
            ~Food();
            Food &operator=(const Food &other);

            bool take(unsigned short foodQuantity);

            void refill(unsigned short foodQuantity);

            int getQuantity() const;

            std::string getName() const;

            void setName(std::string name);

        protected:
        private:
            std::string _name;
            int _quantity;
    };
}

#endif /* !FOOD_HPP_ */
