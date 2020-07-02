/*
** EPITECH PROJECT, 2020
** IFood
** File description:
** IFood
*/

#ifndef IFOOD_HPP_
#define IFOOD_HPP_

#include <string>

namespace Plazza {
    class IFood {
        public:
            ~IFood() {};

            virtual bool take(unsigned short foodQuantity) = 0;

            virtual void refill(unsigned short foodQuantity) = 0;

            virtual int getQuantity() const = 0;

            virtual std::string getName() const = 0;

        protected:
        private:
    };
}

#endif /* !IFOOD_HPP_ */
