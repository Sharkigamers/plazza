/*
** EPITECH PROJECT, 2020
** *
** File description:
** Server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <stdint.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <exception>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <signal.h>
#include <arpa/inet.h>

#include "Process.hpp"

#define MAX_CLIENTS 100
#define MESSAGE_LENGTH 1024

namespace Plazza {
    
    enum messageType {
        NONE = -1,
        RESPONSE_STATUS,
        STATUS,
        PIZZA_REQUEST,
        REQUEST_COOKS_STATUS,
        EXIT_KITCHEN,
        ACCEPT_PIZZA,
        REFUSE_PIZZA,
        FINISH_PIZZA
    };

#pragma pack(1)
    typedef struct message {
        messageType type;
        char message[1024];
        int value;
        pid_t pid;
    } message_t;
#pragma pack(0)

    class Server : public ITask {
        public:
            enum ServerAction {
                ASK_STATUS,
                SEND_PIZZA,
                CREATE_KITCHEN
            };

        public:
            Server(uint16_t port = 4242, int cooksPerKitchen = 5);
            ~Server();

            int getServerFd() const { return _servFd; }
            uint16_t getPort() const { return _port; }

            int acceptClient();
            void initServer();
            void loop();
            void launch() final { loop(); };
            void sendMessage(int clientFd, Plazza::message_t *message) const;
            void sendMessageFromPid(pid_t pid, Plazza::message_t *message);
            ssize_t recieveMessage(int toRecieve);
            void pushServerAction(ServerAction action, int value);
            void checkServerActionQueue(); 
            void getKitchenStatus() const;
            Plazza::message_t getLastResponse();
            void pushPid(Process newProcess);
            std::vector<Process> getListPid() const { return _listProcess; }
            size_t nbProccess() const { return _listProcess.size(); }
            bool isResponseStatusOk() const { return (_response.size() == _listProcess.size()); }
            Plazza::message_t getKitchenResponse(pid_t toWait);

        protected:
        private:
            uint16_t _port;
            int _servFd;
            int _cooksPerKitchen;
            struct sockaddr_in addr_in;
            std::vector<int> _clients;
            std::vector<std::pair<ServerAction, int> > _taskAction;
            std::vector<Plazza::message_t> _response;
            std::vector<Process> _listProcess;
            std::vector<std::pair<pid_t, int> > _fdMap;
            int _toSendIndex = 0;
    };

    class ServerError : public std::exception {
        public:
            explicit ServerError(const std::string &desc = "");
            ~ServerError();

            const char *what() const throw() final;

        private:
            std::string _description;
    };
}

static inline Plazza::message_t *createMessage(Plazza::messageType type, char current[MESSAGE_LENGTH], pid_t pid, int value)
{
    Plazza::message_t *newMessage = static_cast<Plazza::message_t*>(calloc(1, sizeof(Plazza::message_t)));

    newMessage->type = type;
    newMessage->pid = pid;
    newMessage->value = value;
    for (size_t i = 0; i < MESSAGE_LENGTH && current[i] != '\0'; i++)
        newMessage->message[i] = current[i];
    return newMessage;
}

static inline int max(std::vector<int> clients, int serv_fd)
{
    int max = 0;

    for (auto &x : clients) {
        if (x > max)
            max = x;
    }
    max = (serv_fd > max ? serv_fd : max);
    return max;
}

#endif /* !SERVER_HPP_ */
