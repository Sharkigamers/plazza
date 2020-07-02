/*
** EPITECH PROJECT, 2020
** input
** File description:
** Input
*/

#ifndef INPUT_HPP_
#define INPUT_HPP_
#include <iostream>
#include <vector>
#include <tuple>

#include "Utils.hpp"

static inline const std::string pizzaReg ="^((\t|\n|\v|\f|\r| )+)?[A-z]+(((\t|\n|\v|\f|\r| )+)?)(S|M|L|XL|XXL)(((\t|\n|\v|\f|\r| )+)?)x[1-9][0-9]*(((\t|\n|\v|\f|\r| )+)?)((;?((\t|\n|\v|\f|\r| )+)?[A-z]+(((\t|\n|\v|\f|\r| )+)?)(S|M|L|XL|XXL)(((\t|\n|\v|\f|\r| )+)?)x[1-9][0-9]*(((\t|\n|\v|\f|\r| )+)?))?)+?$";
static inline const std::regex spaceReg("(\t|\n|\v|\f|\r| )+");

typedef std::tuple<std::string, std::string, std::string>tripleString;
typedef std::vector<tripleString>pizzaList;

class Input {
    public:
        Input();
        ~Input();

        std::string getInput() const;
        std::string cleanInput(std::string &) const;
        tripleString addInList(std::string &, std::string &, std::string &);
        std::vector<std::string> parseInput(std::string &, char);
        std::vector<std::string> getTabInput(std::string &);
        pizzaList strToTupleTab(std::vector<std::string> &);
        pizzaList multipleOccurences(pizzaList) const;
        uint16_t extractNum(std::string) const;
    protected:
    private:
};


namespace Plazza {
    class PlazzaInputError : public std::exception {
        public:
            typedef enum ErrorType_t {
                QUIT, 
                SYNTAX            
            } ErrorType;

            explicit PlazzaInputError(ErrorType, const std::string &desc = "");
            ~PlazzaInputError();

            const char *what() const throw() final;
            ErrorType getErrorType() const;

        private:
            std::string _description;
            ErrorType _error;
    };
}

#endif /* !INPUT_HPP_ */
