/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** Yield
*/

#ifndef YIELD_HPP_
#define YIELD_HPP_

#include <pthread.h>

class Yield {
    public:
        ~Yield() = delete;

        static int yield() { return pthread_yield(); }

    protected:
    private:
};

#endif /* !YIELD_HPP_ */
