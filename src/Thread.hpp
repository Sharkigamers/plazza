/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** IThread
*/

#ifndef THREAD_HPP_
#define THREAD_HPP_

#include <pthread.h>
#include <utility>
#include <exception>
#include <string>

using startRoutine = void *(*) (void *);

class Thread {
    public:
        enum ThreadStatus {
            NOT_STARTED,
            WAITING,
            STARTED,
            STOPPED
        };
    public:
        explicit Thread(startRoutine, void *);
        ~Thread();

        pthread_t getIndex() const { return this->_index; }
        ThreadStatus getThreadStatus() const { return _status; }
        void setThreadStatus(ThreadStatus status) { _status = status; }

        void start();
        void run();
        void *join();
        void stop() {pthread_exit(nullptr); }
        void updateArg(void *args) { _startupRoutine.second = args; }

    protected:
    private:
        pthread_t _index;
        ThreadStatus _status;
        std::pair<startRoutine, void *>_startupRoutine;
};

class ThreadError : public std::exception {
    public:
        explicit ThreadError(const std::string &desc = "");
        ~ThreadError();

        const char *what() const throw() final;

    private:
        std::string _description;
};


#endif /* !THREAD_HPP_ */
