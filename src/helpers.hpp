/**
 *  Alex Carbajal
 *  CPT_S 355, Spring 2024
 *  Homework 4
 */

#pragma once

#include <random>

#define NUM_PHILOSOPHERS 5

// chopstick states
#define CHOPSTICK_DOWN 1
#define CHOPSTICK_UP 0

// flipping a coin
#define HEADS 0 // get/release the chopsticks
#define TAILS 1 // don't interact with the chopsticks

// time in seconds
#define TOTAL_RUN_TIME 30.0  // the philosophers will eat for this amount of time
#define IS_HUNGRY_TIME 6.0   // the philosopher gets hungry after this amount of time
#define IS_STARVING_TIME 4.0 // the philosopher is starving after this amount of time spent hungry

// range to generate random numbers in
#define MIN_RANGE 1.0
#define MAX_RANGE 5.0

// a philosopher is always in one of these states
enum States
{
    THINKING = 0,
    EATING = 1,
    HUNGRY = 2,
    STARVING = 3 // occurs after being hungry for a certain amount of time
};

// get the left neighbor of id
int getLeft(int id)
{
    return (id - 1 + NUM_PHILOSOPHERS) % NUM_PHILOSOPHERS;
}

// get the right neighbor of id
int getRight(int id)
{
    return (id + 1) % NUM_PHILOSOPHERS;
}

// generate a random double between min and max (time is in milliseconds)
double randomTime(double min, double max)
{
    std::random_device seed;                          // random seed
    std::mt19937 generate(seed());                    // seed the random number generation engine
    std::uniform_real_distribution<> range(min, max); // set the range

    return range(generate) * 1e6; // generate a random number within the range
}
