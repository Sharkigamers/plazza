/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** ServerClient
*/

#include "ServerClient.hpp"

Plazza::ServerClient::ServerClient(int serverFd, int serverPort)
    : _servFd(serverFd), _port(serverPort)
{
    connectToServer();
}

Plazza::ServerClient::~ServerClient()
{
    close(_servFd);
    close(_clientFd);
}

void Plazza::ServerClient::connectToServer()
{
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(_port);
 
    if ((_clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        std::cerr << "Socket creation error." << std::endl;
        exit(84);
    } 
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) { 
        std::cerr << "Invalid address/ Address not supported." << std::endl; 
        exit(84);
    } 
   
    if (connect(_clientFd,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        std::cerr << "Connection Failed." << std::endl; 
        exit(84);
    }
    std::cout << "Connected" << std::endl;
}

Plazza::message_t Plazza::ServerClient::pollEvent() const
{
    Plazza::message_t message;
    fd_set readfds;
    struct timeval timeout = {static_cast<long>(0.2), 200};

    FD_ZERO(&readfds);
    FD_SET(_clientFd, &readfds);
    if (select(_clientFd + 1, &readfds, nullptr, nullptr, &timeout) == -1)
        throw Plazza::ClientError("could not recieve server message : " + std::string(strerror(errno)) + " : " + std::to_string(_clientFd));
    if (FD_ISSET(_clientFd, &readfds)) {
        if (read(_clientFd, &message, sizeof(Plazza::message_t)) == -1)
            throw Plazza::ClientError("could not recieve server message : " + std::string(strerror(errno)) + " : " + std::to_string(_clientFd));
    } else 
        message.type = NONE;
    return message;
}

void Plazza::ServerClient::sendResponse(Plazza::message_t *message) const 
{
    if (send(_clientFd, message, sizeof(Plazza::message_t), 0) == -1)
        throw Plazza::ClientError("could not send message to server");
}

Plazza::message_t &Plazza::ServerClient::operator<<(Plazza::message_t &in)
{
    in = this->pollEvent();
    return in;
}

Plazza::ClientError::ClientError(const std::string &desc) : std::exception()
{ 
    if (desc == "") {
        _description = std::string("Unknown client error.");
    } else {
        _description = std::string("Plazza client error : " + desc);
    }
}

Plazza::ClientError::~ClientError()
{
}

const char *Plazza::ClientError::what() const throw()
{
    return _description.c_str();
}