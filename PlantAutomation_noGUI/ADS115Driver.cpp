/**
 * @file ADS115Driver.cpp
 *
 * @brief A simple program to demonstrate the usage of the ADS1115 ADC driver.
 */

#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <chrono>
#include <thread>

#include "ADS1115.h"

/**
 * @brief Main function for the ADS1115 ADC driver demonstration.
 * @return 0 on successful execution.
 */
int main() {
    // Create instances of ADS1115 ADC (16-bit) for different channels
    ADS1115 adc(0x48, ADS1115::Mux::AIN0_GND);
    ADS1115 adc1(0x48, ADS1115::Mux::AIN1_GND);
    ADS1115 adc2(0x48, ADS1115::Mux::AIN2_GND);
    ADS1115 adc3(0x48, ADS1115::Mux::AIN3_GND);

    // Read analog inputs 100 times
    for (int i = 0; i < 100; i++) {
        std::cout << "AIN0: " << std::dec << adc.read0() << std::endl;
        std::cout << "AIN1: " << std::dec << adc1.read1() << std::endl;
        std::cout << "AIN2: " << std::dec << adc2.read2() << std::endl;
        std::cout << "AIN3: " << std::dec << adc3.read3() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
