/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** ThreadPool
*/

#include "ThreadPool.hpp"
#include <iostream>
ThreadPool::ThreadPool(size_t max, startRoutine startRoutine, void *arg)
    : _max(max), _startRoutine(startRoutine), _arg(arg)
{
    for (size_t i = 0; i < max; i++) {
        try
        {
            _threads.push_back(new Thread(startRoutine, arg));
            _threads[i]->setThreadStatus(Thread::STOPPED);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

ThreadPool::~ThreadPool()
{
}

size_t ThreadPool::getListAvailable() const
{
    size_t res = 0;

    for (auto &x : _threads) {
        if (x->getThreadStatus() == Thread::STOPPED)
            res++;
    }
    return res;
}

void ThreadPool::runTask(void *newArg)
{
    for (auto &x : _threads) {
        if (x->getThreadStatus() == Thread::STOPPED) {
            x->setThreadStatus(Thread::ThreadStatus::STARTED);
            Thread *tmp = new Thread([](void *arg) -> void * {
                Thread *newThread = static_cast<Thread*>(arg);
                newThread->run();
                newThread->setThreadStatus(Thread::STOPPED);
                return nullptr;
            }, x);
            tmp->start();
            break;
        }
    }
}