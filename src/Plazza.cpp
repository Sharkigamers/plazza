/*
** EPITECH PROJECT, 2020
** Plazza
** File description:
** Plazza
*/

#include "Plazza.hpp"
#include "Thread.hpp"

#include <tuple>

Plazza::Reception::Reception(int ac, char **av)
    : _server(Server(4242, std::atoi(av[2])))
{
    if (ac != 4) {
        throw PlazzaArgumentError("Wrong number of arguments.");
    } else {
        try
        {
            try
            {
                _multiplier = std::atof(av[1]);
            }
            catch(const std::exception& e)
            {
                throw PlazzaArgumentError("Invalid multiplier.");
            }
            if (_multiplier <= 0)
                throw PlazzaArgumentError("Invalid multiplier.");

            try
            {
                _cooksPerKitchen = std::atoi(av[2]);
            }
            catch(const std::exception& e)
            {
                throw PlazzaArgumentError("Invalid number of cooks per kitchen.");
            }
            if (_cooksPerKitchen <= 0)
                throw PlazzaArgumentError("Invalid number of cooks per kitchen.");

            try
            {
                _stockRefiller = std::atof(av[3]);
            }
            catch(const std::exception& e)
            {
                throw PlazzaArgumentError("Invalid stock refilling timing.");
            }
            if (_stockRefiller <= 0)
                throw PlazzaArgumentError("Invalid stock refilling timing.");

        }
        catch (const PlazzaArgumentError& e)
        {
            throw e;
        }
    }
}

Plazza::Reception::~Reception()
{
}

void Plazza::Reception::getAndPrintKitchensStatus()
{
    _listResponse.clear();
    if (_server.nbProccess() == 0) {
        std::cout << "There is no kitchen currently running." << std::endl;
        return;
    } else
        _server.pushServerAction(Plazza::Server::ServerAction::ASK_STATUS, -1);
    // while (_server.isResponseStatusOk() != true)
    //     usleep(10);
    // Plazza::message_t response;
    // while (1) {
    //     response = _server.getLastResponse();
    //     if (response.type == NONE)
    //         break;
    //     else
    //         _listResponse.push_back(response);
    // }
}

void Plazza::Reception::sendPizzaToKitchen(Plazza::Cooks::PizzaType type)
{
    if (_server.getListPid().size() == 0) {
        sleep(1);
    }
    std::cout << "Sending " << Plazza::Kitchen::pack(type) << std::endl;
    _server.pushServerAction(Plazza::Server::ServerAction::SEND_PIZZA, type);
}

void Plazza::Reception::dispatchCommand(std::string &input)
{
    Input in;
    std::vector<std::string> tab = in.getTabInput(input);
    pizzaList listPizza = in.strToTupleTab(tab);

    std::cout << "Command size: " << listPizza.size() << std::endl;
    for (const auto &x : listPizza) {
        char *tmp = strdup(std::get<0>(x).c_str());
        Plazza::Cooks::PizzaType type = Plazza::Kitchen::unpack(tmp);
        sendPizzaToKitchen(type);
    }
}

int Plazza::Reception::loop()
{
    Input in;
    std::string input;
    BMutex mutex;
    Thread *serverThread = new Thread([](void *arg) -> void * {
        Plazza::Reception *reception = static_cast<Plazza::Reception*>(arg);
        try
        {
            reception->_server.loop();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        
        return nullptr;
    }, this);

    serverThread->start();
    while (true != false) {
        try
        {
            input = in.getInput();

            if (input == "status") {
                this->getAndPrintKitchensStatus();
            } else {
                dispatchCommand(input);
            }
        }
        catch(const Plazza::PlazzaInputError& e)
        {
            if (e.getErrorType() == Plazza::PlazzaInputError::ErrorType::QUIT)
                break;
            else
                std::cerr << e.what() << std::endl;
        }
    }
    return 0;
}

Plazza::PlazzaArgumentError::PlazzaArgumentError(const std::string &desc) : std::exception()
{ 
    if (desc == "") {
        _description = std::string("Unknown argument error.");
    } else {
        _description = std::string("Plazza argument error : " + desc);
    }
}

Plazza::PlazzaArgumentError::~PlazzaArgumentError()
{
}

const char *Plazza::PlazzaArgumentError::what() const throw()
{
    return _description.c_str();
}