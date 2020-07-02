/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** Process
*/

#include "Process.hpp"

#include <iostream>

Process::Process(ITask *task)
    : _task(task)
{
    pid_t newKitchen;

    std::cout << "Creating a kitchen" << std::endl;
    newKitchen = fork();
    if (newKitchen == -1) {
        std::cerr << "An error has occured while creating a kitchen" << std::endl;
    } else if (newKitchen == 0) {
        this->run();
        exit(EXIT_SUCCESS);
    }
    std::cout << "Kitchen n°" << newKitchen << " created successfully." << std::endl;
    _pid = newKitchen;
}

Process::~Process()
{
}

void Process::run() const
{
    _task->launch();
}