/**
 * @file WaterDriver.cpp
 *
 * @brief A simple program to demonstrate the usage of the WaterPump class.
 *
 * To compile this program, use the following command:
 *
 *     g++ -I/home/kpf5297/Code/ManualControl WaterDriver.cpp WaterPump.cpp -o WaterDriver -lgpiod -lrt -lpthread
 */

#include "WaterPump.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>

/**
 * @brief The main function of the program.
 * @return 0 on successful execution.
 */
int main() {
    // Create the water pump
    WaterPump waterPump(17, 5, 5);

    // Activate the water pump
    waterPump.activate();

    // Wait for the water pump to finish
    std::this_thread::sleep_for(std::chrono::seconds(waterPump.getActivationDuration()));

    // Deactivate the water pump
    waterPump.deactivate();

    return 0;
}
