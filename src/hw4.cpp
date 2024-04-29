/**
 *  Alex Carbajal
 *  CPT_S 355, Spring 2024
 *  Homework 4
 */

#include <iostream>
#include <string>

#include "philosopher.hpp"
#include "syncro.hpp"
#include "helpers.hpp"

int main(int argc, char *argv[])
{
    Philosopher *philosophers[NUM_PHILOSOPHERS];
    Syncro syncro;
    // names for the philosophers
    const std::string nameArray[] = {"Yoda", "Obi-Wan", "Rey", "Kanan", "Leia", "Luke", "Ahsoka",
                                     "Mace Windu", "Ezra", "Palpatine", "Anakin", "Kylo Ren", "Dooku",
                                     "Kit Fitso", "Luminara", "Plo Koon", "Revan", "Thrawn", "Zeb", "Sabine"};

    // allocate philosophers[]
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        philosophers[i] = new Philosopher(nameArray[i], i, syncro.getChopstick(getLeft(i)), syncro.getChopstick(getRight(i)), syncro);
    }

    // free the memory for philosophers[]
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        delete philosophers[i];
    }

    return 0;
}
