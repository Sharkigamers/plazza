/*
** EPITECH PROJECT, 2020
** Plazza
** File description:
** Plazza
*/

#ifndef PLAZZA_HPP_
#define PLAZZA_HPP_

#include <exception>
#include <string>
#include <vector>
#include <memory>
#include "kitchen/Kitchen.hpp"
#include "Input.hpp"
#include "Server.hpp"

namespace Plazza {
    class Reception {
        public:
            Reception(int ac, char **av);
            ~Reception();

            static pid_t createKitchen(int serverFd, int numberCooks, int serverPort);
            int loop();
            void dispatchCommand(std::string &input);
            void getAndPrintKitchensStatus();
            void sendPizzaToKitchen(Plazza::Cooks::PizzaType type);

        protected:
        private:
            double _multiplier;
            int _cooksPerKitchen;
            double _stockRefiller;
            Server _server;
            std::vector<Plazza::message_t> _listResponse;
    };

    class PlazzaArgumentError : public std::exception {
        public:
            explicit PlazzaArgumentError(const std::string &desc = "");
            ~PlazzaArgumentError();

            const char *what() const throw() final;

        private:
            std::string _description;
    };
}

#endif /* !PLAZZA_HPP_ */
