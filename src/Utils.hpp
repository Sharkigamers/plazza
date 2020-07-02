/*
** EPITECH PROJECT, 2020
** Utils
** File description:
** Utils
*/

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <regex>

namespace Plazza {
    class Utils {
        public:
            Utils();
            ~Utils();

            /* utils */

                /* regex */

                bool verifyLine(const std::string lineToVerify, const std::string regex);

                /* space */

               std::string clearSpaces(std::string str);
               std::string &ltrim(std::string &str, const std::string &chars = "\t\n\v\f\r ");
               std::string &rtrim(std::string &str, const std::string &chars = "\t\n\v\f\r ");
               std::string &trim(std::string &str, const std::string &chars = "\t\n\v\f\r ");

        protected:
        private:
    };
}

#endif /* !UTILS_HPP_ */
