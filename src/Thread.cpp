/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** Thread
*/

#include "Thread.hpp"

#include <errno.h>
#include <string.h>
#include <iostream>

Thread::Thread(startRoutine start, void *args)
    : _status(NOT_STARTED), _startupRoutine(std::make_pair(start, args))
{
}

Thread::~Thread()
{
}

void Thread::start()
{
    setThreadStatus(STARTED);
    if (pthread_create(&_index, nullptr, _startupRoutine.first, _startupRoutine.second) != 0)
        throw ThreadError(std::string(strerror(errno)));
}

void Thread::run()
{
    _startupRoutine.first(_startupRoutine.second);
}

void *Thread::join()
{
    void *returnVal;

    pthread_join(_index, &returnVal);
    setThreadStatus(STOPPED);
    return returnVal;
}

ThreadError::ThreadError(const std::string &desc) : std::exception()
{ 
    if (desc == "") {
        _description = std::string("Unknown thread error.");
    } else {
        _description = std::string("Thread error : " + desc);
    }
}

ThreadError::~ThreadError()
{
}

const char *ThreadError::what() const throw()
{
    return _description.c_str();
}