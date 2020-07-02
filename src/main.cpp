/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** main
*/

#include "Plazza.hpp"
#include "Thread.hpp"
#include <iostream>
#include <thread>

int main(int ac, char **av)
{
    try
    {
        Plazza::Reception reception(ac, av);
        return reception.loop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
