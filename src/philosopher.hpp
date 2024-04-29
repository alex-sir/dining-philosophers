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
    double eatTime;    // total amount of time spent eating
    double thinkTime;  // total amount of time spent thinking
    double starveTime; // total amount of time spent starving (wants to eat but can't)
    double currentThinkTime;
    double currentStarveTime;
    Syncro &syncro;          // Syncro object that is shared among all philosophers
    Chopstick &left, &right; // identify the connections with the set of chopsticks
    std::thread mainThread;

    // philosopher is thinking
    void think(int id);
    // attempt to take the left and right chopsticks
    void take_chopsticks(int id);
    // perform the eating action
    void eat(int id);
    // put down the left and right chopsticks so other philosophers can utilize them
    void release_chopsticks(int id);
    // check status of a philosopher neighbor and potentially change status - used to know when to request the chopsticks
    void test(int id);
    // print out information about the philosopher
    void printStatistics(void);

public:
    Philosopher(std::string newName, int newId, Chopstick &newLeft, Chopstick &newRight, Syncro &newSyncro)
        : syncro(newSyncro), left(newLeft), right(newRight), mainThread(&Philosopher::run, this)
    {
        name = newName;
        id = newId;
        state = THINKING;
        eatTime = 0.0;
        thinkTime = 0.0;
        starveTime = 0.0;
        currentThinkTime = 0.0;
        currentStarveTime = 0.0;
    }

    ~Philosopher()
    {
        mainThread.join();
    }

    void run(void);
};

void Philosopher::think(int id)
{
    if (state == THINKING && currentThinkTime >= IS_HUNGRY_TIME)
    {
        currentThinkTime = 0.0;

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

        double randomThinkTime = randomTime(MIN_RANGE, MAX_RANGE);
        usleep(randomThinkTime); // pause for a certain amount of time
        currentThinkTime += randomThinkTime;
        thinkTime += randomThinkTime;
    }
}

void Philosopher::take_chopsticks(int id)
{
    if (state == HUNGRY || state == STARVING)
    {
        test(id);

        // philosopher couldn't eat, add to time until starving
        if (state != EATING)
        {
            double randomStarveTime = randomTime(MIN_RANGE, MAX_RANGE);

            usleep(randomStarveTime);
            currentStarveTime += randomStarveTime;
            if (state == STARVING)
            {
                starveTime += randomStarveTime;
            }

            // begin starving
            if (state != STARVING && currentStarveTime >= IS_STARVING_TIME)
            {
                state = STARVING;

                outputMutex.lock();
                std::cout << "philosopher " << id << " is starving!\n";
                outputMutex.unlock();

                starveTime += (currentStarveTime - IS_STARVING_TIME);
            }

            // starving for too long - force the philosopher to eat
            if (currentStarveTime >= IS_STARVING_TIME * 3)
            {
                while (state != EATING)
                {
                    test(id);
                }
            }
        }
    }
}

void Philosopher::eat(int id)
{
    if (state == EATING)
    {
        outputMutex.lock();
        std::cout << "philosopher " << id << " is eating\n";
        outputMutex.unlock();

        double randomEatTime = randomTime(MIN_RANGE, MAX_RANGE);
        usleep(randomEatTime);
        eatTime += randomEatTime;
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

    if ((state == HUNGRY || state == STARVING) &&
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

        // reset the starve time
        if (currentStarveTime >= 0.0)
        {
            currentStarveTime = 0.0;
        }
    }
}

void Philosopher::printStatistics(void)
{
    std::cout << "\n*** Philosopher " << id << " Statistics ***\n";
    std::cout << "Eating Time:   " << eatTime << "ms\n";
    std::cout << "Thinking Time: " << thinkTime << "ms\n";
    std::cout << "Starving Time: " << starveTime << "ms\n\n";
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

    usleep(1 * 1e6);
    outputMutex.lock();
    printStatistics();
    outputMutex.unlock();
}
