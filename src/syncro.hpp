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
    Chopstick chopsticks[NUM_PHILOSOPHERS];
    int states[NUM_PHILOSOPHERS] = {THINKING}; // the state of each of the philosophers

public:
    // getters
    Chopstick &getChopstick(int id);
    int getState(int id);

    // setters
    void setState(int id, int newState);

    // validate conditions and put down chopstick id
    void putDownChopstick(int id);
    // validate conditions and pick up chopstick id
    void pickUpChopstick(int id);
};

Chopstick &Syncro::getChopstick(int id)
{
    return chopsticks[id];
}

int Syncro::getState(int id)
{
    return states[id];
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
