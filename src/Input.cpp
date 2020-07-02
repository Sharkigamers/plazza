/*
** EPITECH PROJECT, 2020
** input
** File description:
** Input
*/

#include "Input.hpp"

Input::Input()
{

}

Input::~Input()
{

}

Plazza::PlazzaInputError::PlazzaInputError(ErrorType error, const std::string &desc) : _error(error)
{
    if (desc == "") {
        _description = std::string("Unknown argument error.");
    } else {
        _description = std::string("Plazza argument error : " + desc);
    }
}

Plazza::PlazzaInputError::ErrorType Plazza::PlazzaInputError::getErrorType() const
{
    return (_error);
}

Plazza::PlazzaInputError::~PlazzaInputError()
{

}

const char *Plazza::PlazzaInputError::what() const throw()
{
    return _description.c_str();
}

std::string Input::getInput() const
{
    std::string command;
    Plazza::Utils utils;

    if (!std::getline(std::cin, command))
        throw Plazza::PlazzaInputError(Plazza::PlazzaInputError::ErrorType::QUIT, "Getline Failed");
    if (command == "status");
    else if (!utils.verifyLine(command, pizzaReg))
        throw Plazza::PlazzaInputError(Plazza::PlazzaInputError::ErrorType::SYNTAX, "Wrong arguments");
    return(command);
}

std::string Input::cleanInput(std::string &command) const
{
    Plazza::Utils utils;
    std::string res = std::regex_replace(command, spaceReg, " ");

    if (res[0] == ' ')
        res = utils.ltrim(res);
    if (res[res.length()] == ' ')
        res = utils.rtrim(res);
    return (res);
}

std::vector<std::string> Input::parseInput(std::string &command, char delim)
{
    std::vector<std::string> res;
    std::string tmp;
    std::istringstream tokenStream(command);

    while (std::getline(tokenStream, tmp, delim))
        res.push_back(cleanInput(tmp));
    return (res);
}

std::vector<std::string> Input::getTabInput(std::string &command)
{
    std::vector<std::string> res;
    std::vector<std::string> first = parseInput(command, ';');
    std::string tmp;

    for (size_t i = 0; i != first.size(); i++) {
        std::istringstream tokenStream(first[i]);
        while (std::getline(tokenStream, tmp, ' '))
            res.push_back(cleanInput(tmp));
    }
    return (res);
}

pizzaList Input::strToTupleTab(std::vector<std::string> &tabString)
{
    pizzaList res;
    std::string formatNumberPizza;
    int numberPizza;

    for (size_t i = 0; i != tabString.size(); i += 3) {
        formatNumberPizza = tabString[i + 2];
        numberPizza = std::stoi(tabString[i + 2].erase(0, 1));
        for (int j = 0; j < numberPizza; ++j)
            res.push_back(std::make_tuple(tabString[i],tabString[i + 1], formatNumberPizza));
    }
    return (res);
}

uint16_t Input::extractNum(std::string nb) const
{
    size_t i = 0;
    u_int16_t res;

    for (; i < nb.length(); i++ ) {
         if (isdigit(nb[i]))
            break;
    }
    nb = nb.substr(i, nb.length()-i);
    res = atoi(nb.c_str());
    return (res);
}

pizzaList Input::multipleOccurences(pizzaList pizzas) const
{
    u_int16_t nbPizzas = 0;
    pizzaList res;

    for (size_t i = 0; i != pizzas.size(); ++i) {
        nbPizzas = extractNum(std::get<2>(pizzas[i]));
        if (nbPizzas > 0)
            for (size_t j = 0; j != nbPizzas; ++j)
                res.push_back(pizzas[i]);
    }
    return(res);
}