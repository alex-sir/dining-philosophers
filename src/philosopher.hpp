/**
 *  Alex Carbajal
 *  CPT_S 355, Spring 2024
 *  Homework 4
 */

#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <cmath>
#include <unistd.h>

#include "syncro.hpp"
#include "chopstick.hpp"
#include "helpers.hpp"

std::mutex outputMutex; // synchronize cout print statements

class Philosopher : std::thread
{
private:
    std::string name;
    int id;
    int state;
    double thinkTime;        // total amount of time spent thinking
    double starvingTime;     // total amount of time spent starving (wants to eat but can't)
    Syncro &syncro;          // Syncro object that is shared among all philosophers
    Chopstick &left, &right; // identify the connections with the set of chopsticks
    std::thread mainThread;

    // philosopher is thinking
    void think(int id);
    // toss a coin to see if the philosopher will acquire the chopsticks
    void take_chopsticks(int id);
    // perform the eating action
    void eat(int id);
    // toss a coin to see if the philosopher will release the chopsticks
    void release_chopsticks(int id);
    // check status of a philosopher neighbor and potentially change status
    // can be used to know when to request the chopsticks
    void test(int id);

public:
    Philosopher(std::string newName, int newId, Chopstick &newLeft, Chopstick &newRight, Syncro &newSyncro)
        : syncro(newSyncro), left(newLeft), right(newRight), mainThread(&Philosopher::run, this)
    {
        name = newName;
        id = newId;
        state = THINKING;
        thinkTime = 0.0;
        starvingTime = 0.0;
    }

    ~Philosopher()
    {
        mainThread.join();
    }

    void run(void);
};

void Philosopher::think(int id)
{
    if (state == THINKING && std::fmod(thinkTime, IS_HUNGRY_TIME) == 0)
    {
        state = HUNGRY;
        syncro.setState(id, state);

        outputMutex.lock();
        std::cout << "philosopher " << id << " is hungry\n";
        outputMutex.unlock();
    }
    else if (state == THINKING)
    {
        outputMutex.lock();
        std::cout << "philosopher " << id << " is thinking\n";
        outputMutex.unlock();

        usleep(THINKING_TIME); // pause for a certain amount of time
        thinkTime += THINKING_TIME;
    }
}

void Philosopher::take_chopsticks(int id)
{
    if (state == HUNGRY)
    {
        test(id);
    }
}

void Philosopher::eat(int id)
{
    if (state == EATING)
    {
        outputMutex.lock();
        std::cout << "philosopher " << id << " is eating\n";
        outputMutex.unlock();

        usleep(EATING_TIME);
    }
}

void Philosopher::release_chopsticks(int id)
{
    if (state == EATING)
    {
        state = THINKING;
        syncro.setState(id, state);

        left.unlockChopstick();
        right.unlockChopstick();

        outputMutex.lock();
        std::cout << "philosopher " << id << " finished eating\n";
        outputMutex.unlock();
    }
}

void Philosopher::test(int id)
{
    int leftNeighbor = getLeft(id);
    int rightNeighbor = getRight(id);

    if (state == HUNGRY &&
        syncro.getState(leftNeighbor) != EATING && syncro.getState(rightNeighbor) != EATING)
    {
        state = EATING;
        syncro.setState(id, state);

        left.lockChopstick();
        right.lockChopstick();

        outputMutex.lock();
        std::cout << "philosopher " << id << " picked up chopsticks "
                  << leftNeighbor << " & " << rightNeighbor << std::endl;
        outputMutex.unlock();
    }
}

void Philosopher::run(void)
{
    // for keeping track of time
    auto runStart = std::chrono::high_resolution_clock::now();
    auto runEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> currentTime = runEnd - runStart;

    // let the philosopher dine for a certain amount of time TOTAL_RUN_TIME
    while (currentTime.count() < TOTAL_RUN_TIME)
    {
        // perform the actions
        think(id);
        take_chopsticks(id);
        eat(id);
        release_chopsticks(id);

        // get the current elapsed time
        runEnd = std::chrono::high_resolution_clock::now();
        currentTime = runEnd - runStart;
    }
}
