/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** Process
*/

#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include <sys/types.h>
#include <unistd.h>

#include "ITask.hpp"

class Process {
    public:
        Process(ITask *task);
        ~Process();

        pid_t getPid() const { return _pid; }
        bool operator==(const Process &other) { return (this->getPid() == other.getPid());}

    protected:
    private:
        pid_t _pid;
        ITask *_task;
        void run() const;
};

#endif /* !PROCESS_HPP_ */