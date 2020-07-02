/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** Server
*/

#include "Server.hpp"
#include <iostream>
#include <algorithm>
#include "BMutex.hpp"
#include "kitchen/Kitchen.hpp"
#include "Plazza.hpp"

Plazza::Server::Server(uint16_t port, int cooksPerKitchen)
    : _port(port), _cooksPerKitchen(cooksPerKitchen)
{
    initServer();
}

Plazza::Server::~Server()
{
    close(_servFd);
    _servFd = 0;
}

void Plazza::Server::initServer()
{
    struct protoent *proto = getprotobyname("TCP");
    struct sockaddr_in addr_in;

    addr_in.sin_addr.s_addr = INADDR_ANY;
    addr_in.sin_port = htons(_port);
    addr_in.sin_family = AF_INET;
    
    _servFd = socket(AF_INET, SOCK_STREAM, proto->p_proto); 
    if (_servFd == -1)
        throw Plazza::ServerError("server failed to start.");
    if (bind(_servFd, (struct sockaddr *)&addr_in, sizeof(addr_in)) == -1)
        throw Plazza::ServerError("server failed to start.");
    if (listen(_servFd, MAX_CLIENTS) == -1)
        throw Plazza::ServerError("server failed to start.");
    std::cout << "Server started" << std::endl;
}

int Plazza::Server::acceptClient()
{
    struct sockaddr_in cli;
    uint64_t size = sizeof(struct sockaddr_in);
    int fd = 0;

    fd = accept(_servFd, (struct sockaddr *)&cli, (socklen_t *)&size);
    if (fd == -1)
        throw Plazza::ServerError("Server failed to accept an incoming connection.");
    std::cout << "Client connected : " <<  inet_ntoa(cli.sin_addr) << "." << std::endl;
    _fdMap.push_back(std::make_pair(_listProcess[_listProcess.size()-1].getPid(), fd));
    return fd;
}

void Plazza::Server::pushPid(Process newProcess)
{
    BMutex mutex;

    mutex.lock();
    _listProcess.push_back(newProcess); 
    mutex.unlock();
}

void Plazza::Server::sendMessage(int clientFd, message_t *message) const
{
    if (send(clientFd, message, sizeof(message_t), 0) == -1)
        throw Plazza::ServerError("Server failed to send a message to " + std::to_string(clientFd) + ":" + strerror(errno));
}

void Plazza::Server::sendMessageFromPid(pid_t pid, message_t *message)
{
    int fd = 0;

    for (auto &x : _fdMap) {
        if (x.first == pid) {
            fd = x.second;
            break;
        }
    }
    if (send(fd, message, sizeof(message_t), 0) == -1)
        throw Plazza::ServerError("Server failed to send a message to " + std::to_string(fd) + ": " + strerror(errno));
}

Plazza::message_t Plazza::Server::getLastResponse()
{
    BMutex mutex;
    Plazza::message_t result;

    result.type = NONE;
    if (_response.size() == 0)
        return result;
    else {
        mutex.lock();
        result = _response.front();
        _response.pop_back();
    }
    return result;
}

ssize_t Plazza::Server::recieveMessage(int toRecieve) 
{
    message_t message;
    ssize_t res = recv(toRecieve, &message, sizeof(message_t), 0);

    if (res == -1)
        throw Plazza::ServerError("Server failed to recieve a message from " + std::to_string(toRecieve) + ".");
    if (res == 0)
        close(toRecieve);
    else {
        try
        {
            if (message.type == RESPONSE_STATUS) {
                std::cout << "Kitchen " << message.pid << " responded: " << message.message << "." << std::endl;
                _response.push_back(message);
            } else if (message.type == ACCEPT_PIZZA) {
                std::cout << "Kitchen " << message.pid << " accepted: " << Plazza::Kitchen::pack(static_cast<Plazza::Cooks::PizzaType>(message.value)) << "." << std::endl;
            } else if (message.type == REFUSE_PIZZA) {
                std::cout << "Kitchen " << message.pid << " refused: " << Plazza::Kitchen::pack(static_cast<Plazza::Cooks::PizzaType>(message.value)) << "." << std::endl;
                _toSendIndex += 1;
                if (_toSendIndex >= _listProcess.size())
                    _taskAction.push_back(std::make_pair(CREATE_KITCHEN, 0));
                _taskAction.push_back(std::make_pair(SEND_PIZZA, message.value));
            } else if (message.type == EXIT_KITCHEN) {
                std::cout << "Kitchen " << message.pid << " closed: " << message.message << std::endl;
                _listProcess.erase(_listProcess.begin());
                _clients.erase(_clients.begin());
            } else if (message.type == FINISH_PIZZA) {
                _toSendIndex = 0;
            }
        }
        catch(const std::exception& e)
        {
            throw e;
        }
    }
    return res;
}

void Plazza::Server::pushServerAction(ServerAction action, int value)
{
    BMutex mutex;

    mutex.lock();
    _taskAction.push_back(std::make_pair(action, value));
    mutex.unlock();
}

Plazza::message_t Plazza::Server::getKitchenResponse(pid_t toWait)
{
    Plazza::message_t message;
    while (1) {
        usleep(10);
        message = getLastResponse();
        if (message.pid == toWait)
            return message;
        else
            _response.push_back(message);
    }
}

void Plazza::Server::checkServerActionQueue()
{
    if (_taskAction.empty() == false) {
        std::pair<Server::ServerAction, int> newAction = _taskAction.front();

        if (newAction.first == ServerAction::ASK_STATUS) {
            this->getKitchenStatus();
        } else if (newAction.first == ServerAction::SEND_PIZZA) {
            if (_clients.size() == 0) {
                _taskAction.push_back(std::make_pair(ServerAction::SEND_PIZZA, newAction.second));
                _taskAction.push_back(std::make_pair(ServerAction::CREATE_KITCHEN, 0));
            } else {
                std::string val = std::to_string(newAction.second);
                char *value = strdup(val.c_str());
                Plazza::message_t *message = createMessage(Plazza::messageType::PIZZA_REQUEST, value, 0, newAction.second);

                sendMessage(_clients[_toSendIndex], message);
                free(message);
                free(value);
            }
        } else if (newAction.first == ServerAction::CREATE_KITCHEN) {
            pushPid(Process(new Plazza::Kitchen(getServerFd(), _cooksPerKitchen, getPort())));
        }
        _taskAction.erase(_taskAction.begin());
    }
}

void Plazza::Server::getKitchenStatus() const
{
    for (auto &x : _clients)
        sendMessage(x, createMessage(Plazza::messageType::REQUEST_COOKS_STATUS, (char*)"Envoie les cooks fdp", 0, 0));
}

void Plazza::Server::loop()
{
    fd_set readfds;
    struct timeval timeout = {static_cast<long>(0.5), 500};
    BMutex mutex;

    while (_servFd != 0) {
        timeout = {static_cast<long>(0.5), 500};
        FD_ZERO(&readfds);
        FD_SET(_servFd, &readfds);
        for (auto x : _clients )
            FD_SET(x, &readfds);
        if (select(max(_clients, _servFd) + 1, &readfds, nullptr, nullptr, &timeout) == -1)
            break;
        if (FD_ISSET(_servFd, &readfds)) {
            mutex.lock();
            _clients.push_back(this->acceptClient());
            mutex.unlock();
        } else {
            try
            {
                for (auto &x : _clients ) {
                    if (FD_ISSET(x, &readfds))
                        if (recieveMessage(x) == 0)
                            _clients.erase(std::find(_clients.begin(), _clients.end(), x));
                }         
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        checkServerActionQueue();
    }
}

Plazza::ServerError::ServerError(const std::string &desc) : std::exception()
{ 
    if (desc == "") {
        _description = std::string("Unknown server error.");
    } else {
        _description = std::string("Plazza server error : " + desc);
    }
}

Plazza::ServerError::~ServerError()
{
}

const char *Plazza::ServerError::what() const throw()
{
    return _description.c_str();
}