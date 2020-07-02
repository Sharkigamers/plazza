/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** ServerClient
*/

#ifndef SERVERCLIENT_HPP_
#define SERVERCLIENT_HPP_

#include <sys/select.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../Server.hpp"

namespace Plazza {
    class ServerClient {
        public:
            ServerClient(int serverFd, int serverPort);
            ~ServerClient();

            void connectToServer();
            Plazza::message_t pollEvent() const;
            void sendResponse(message_t *message) const;

            Plazza::message_t &operator<<(Plazza::message_t &);

        protected:
        private:
            int _servFd;
            int _port;
            int _clientFd;
    };
    class ClientError : public std::exception {
        public:
            explicit ClientError(const std::string &desc = "");
            ~ClientError();

            const char *what() const throw() final;

        private:
            std::string _description;
    };
}

#endif /* !SERVERCLIENT_HPP_ */
