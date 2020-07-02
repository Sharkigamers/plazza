/*
** EPITECH PROJECT, 2020
** Kitchen
** File description:
** Kitchen
*/

#pragma once

#include <fstream>
#include <map>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "Thread.hpp"
#include "ThreadPool.hpp"
#include "Food.hpp"
#include "Cooks.hpp"
#include "ServerClient.hpp"
#include "BMutex.hpp"
#include "Yield.hpp"
#include "../ITask.hpp"

#define EXIT_MESSAGE (char*)"inactivity."

namespace Plazza {
    class Kitchen : public ITask {
        public:
            enum PizzaSize {
                S = 1,
                M = 2,
                L = 4,
                XL = 8,
                XXL = 16
            };

        public:
            Kitchen(int serverFd, int chefNumber, int serverPort);
            ~Kitchen();

            void kitchenWorking();
            void regenFood();

            std::vector<Plazza::Cooks::PizzaType> getQueue() const{
                return (this->_queue);
            }

            bool actionFromMessage(Plazza::message_t *message, pid_t pid);
            void launch() final { kitchenWorking(); };

            bool acceptPizza(Plazza::Cooks::PizzaType pizza);

            bool isWorking() const;

            ServerClient *getClient() const { return _client; }
            std::map<Plazza::Cooks::PizzaType, std::map<std::string, int>> getRecipe() const;

            int takePizza();
            bool takeIngredient(std::string ingredient, int quantity);

            static std::string pack(Plazza::Cooks::PizzaType pizza);
            static Plazza::Cooks::PizzaType unpack(char *pizza);

            unsigned short getFreeCooks();
            unsigned short getCookNumber() const;
            bool actionFromMessage(Plazza::message_t message, pid_t pid);
            void initThreadPool(void) {
                _cooks = new ThreadPool(_cookNumber, [](void *arg) -> void * 
                {
                    Plazza::Kitchen *kitch = static_cast<Plazza::Kitchen *>(arg);

                    Cooks *chief = new Cooks(1, kitch->_recipe, kitch->getCookNumber());
                    chief->cookLoop(kitch);
                    return nullptr;
                }, this);
            }

            Plazza::Food getFoodByName(std::string name);
            void parseContent(std::string file_content);
            void parseFile(std::string path);
            void file_treatement(const char *path, struct dirent *dirt);
            void initRecipe(const char *pathpath = "./assets/pizzas/");

        protected:
        private:
            std::array<Plazza::Food, 9> _ingrediant;
            unsigned short _regenSeconds;

            std::map<Plazza::Cooks::PizzaType, std::map<std::string, int>> _recipe;
            std::vector<Plazza::Cooks::PizzaType> _queue;
            ThreadPool *_cooks;

            unsigned short _cookNumber;
            unsigned short _maxPizza;
            ServerClient *_client;    

            BMutex _locker;
    };
}
