/**
 *  Alex Carbajal
 *  CPT_S 355, Spring 2024
 *  Homework 4
 */

#pragma once

#include "chopstick.hpp"
#include "helpers.hpp"

class Syncro
{
private:
    int states[NUM_PHILOSOPHERS] = {THINKING}; // the state of each of the philosophers
    Chopstick chopsticks[NUM_PHILOSOPHERS];

public:
    // getters
    int getState(int id);
    Chopstick &getChopstick(int id);

    // setters
    void setState(int id, int newState);

    // validate conditions and put down chopstick id
    void putDownChopstick(int id);
    // validate conditions and pick up chopstick id
    void pickUpChopstick(int id);
};

int Syncro::getState(int id)
{
    return states[id];
}

Chopstick &Syncro::getChopstick(int id)
{
    return chopsticks[id];
}

void Syncro::setState(int id, int newState)
{
    states[id] = newState;
}

void Syncro::putDownChopstick(int id)
{
    if (chopsticks[id].getStatus() == CHOPSTICK_UP)
    {
        chopsticks[id].unlockChopstick();
    }
}

void Syncro::pickUpChopstick(int id)
{
    if (chopsticks[id].getStatus() == CHOPSTICK_DOWN)
    {
        chopsticks[id].lockChopstick();
    }
}
