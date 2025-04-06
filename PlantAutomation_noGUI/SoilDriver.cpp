#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <chrono>
#include <thread>

#include "SoilSensor.h"

int main() {
    // Create a soil sensor object
    SoilSensor soilSensor(0x48, ADS1115::Mux::AIN0_GND);

    // Calibrate the sensor
    soilSensor.calibrate();

    // Read the moisture value
    double moisture = soilSensor.readMoisture();

    // Print the moisture value
    std::cout << "Moisture: " << std::fixed << std::setprecision(2) << moisture << "%" << std::endl;

    // Read the moisture value and raw value infinitely
    while (true) {
        // Read the moisture value
        moisture = soilSensor.readMoisture();

        // Print the moisture value
        std::cout << "Moisture: " << std::fixed << std::setprecision(2) << moisture << "%" << std::endl;

        // Read the raw value
        int16_t rawValue = soilSensor.ads1115.read0();

        // Print the raw value
        std::cout << "Raw Value: " << rawValue << std::endl;

        // Wait 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }


    return 0;
}