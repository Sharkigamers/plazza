/*
** EPITECH PROJECT, 2020
** Plazza
** File description:
** Utils
*/

#include "Utils.hpp"

Plazza::Utils::Utils()
{
}

Plazza::Utils::~Utils()
{
}

bool Plazza::Utils::verifyLine(const std::string lineToVerify, const std::string regex)
{
    std::regex myRegex(regex);
    std::smatch match;

    return std::regex_search(lineToVerify, match, myRegex);
}

std::string Plazza::Utils::clearSpaces(std::string str)
{
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
}

std::string &Plazza::Utils::ltrim(std::string &str, const std::string &chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
};
 
std::string &Plazza::Utils::rtrim(std::string &str, const std::string &chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
};
 
std::string &Plazza::Utils::trim(std::string &str, const std::string &chars)
{
    return ltrim(rtrim(str, chars), chars);
};
