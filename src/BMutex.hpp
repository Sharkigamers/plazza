/*
** EPITECH PROJECT, 2020
** bootstrap
** File description:
** BMutex
*/

#ifndef BMUTEX_HPP_
#define BMUTEX_HPP_

#include "IMutex.hpp"

class BMutex : public IMutex {
    public:
        BMutex() {}
        ~BMutex() {};

        void lock() { pthread_mutex_lock(&_mutex); };
        void unlock() { pthread_mutex_unlock(&_mutex); };
        void trylock() { pthread_mutex_trylock(&_mutex); } ;

    protected:
    private:
        pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
};

#endif /* !BMUTEX_HPP_ */
