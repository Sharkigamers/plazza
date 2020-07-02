/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** ThreadPool
*/

#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

#include "Thread.hpp"
#include "pizza/Pizza.hpp"

#include <vector>

class ThreadPool {
    public:
        ThreadPool(size_t max, startRoutine startRoutine, void *arg);
        ~ThreadPool();

        size_t getListAvailable() const;
        void runTask(void *newArg);
        size_t getMaxSize() const { return _max; }

    protected:
    private:
        std::vector<Thread*> _threads;
        size_t _max;
        startRoutine _startRoutine;
        void *_arg;
};

#endif /* !THREADPOOL_HPP_ */
