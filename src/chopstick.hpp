/**
 *  Alex Carbajal
 *  CPT_S 355, Spring 2024
 *  Homework 4
 */

#pragma once

#include <mutex>

#include "helpers.hpp"

class Chopstick
{
private:
    std::mutex chopTex;
    int status;

public:
    Chopstick()
    {
        status = CHOPSTICK_DOWN;
    }

    // getters
    int getStatus(void);

    void lockChopstick();
    void unlockChopstick();
};

int Chopstick::getStatus(void)
{
    return status;
}

void Chopstick::lockChopstick()
{
    chopTex.lock();
    status = CHOPSTICK_UP;
}

void Chopstick::unlockChopstick()
{
    chopTex.unlock();
    status = CHOPSTICK_DOWN;
}
