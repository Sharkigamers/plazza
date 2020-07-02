/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** ITask
*/

#ifndef ITASK_HPP_
#define ITASK_HPP_

class ITask {
    public:
        virtual ~ITask() {};

        virtual void launch() = 0;

    protected:
    private:
};

#endif /* !ITASK_HPP_ */
