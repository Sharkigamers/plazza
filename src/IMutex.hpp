/*
** EPITECH PROJECT, 2020
** bootstrap
** File description:
** IMutex
*/

#ifndef IMUTEX_HPP_
#define IMUTEX_HPP_

#include <pthread.h>

class IMutex {
    public:
        virtual ~IMutex() {};
        
        virtual void lock() = 0;
        virtual void unlock() = 0;
        virtual void trylock() = 0;
};

#endif /* !IMUTEX_HPP_ */
