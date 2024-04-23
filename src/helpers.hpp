/**
 *  Alex Carbajal
 *  CPT_S 355, Spring 2024
 *  Homework 4
 */

#pragma once

#define NUM_PHILOSOPHERS 5

// chopstick states
#define CHOPSTICK_DOWN 1
#define CHOPSTICK_UP 0

// flipping a coin
#define HEADS 0 // get/release the chopsticks
#define TAILS 1 // don't interact with the chopsticks

// time in seconds
#define TOTAL_RUN_TIME 10.0
#define IS_HUNGRY_TIME 3.0 // the philosopher gets hungry after this amount of time

// times in microseconds
#define EATING_TIME 0.5 * 1e6
#define THINKING_TIME 1.0 * 1e6

// a philosopher is always in one of these states
enum States
{
    THINKING = 0,
    EATING = 1,
    HUNGRY = 2
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
