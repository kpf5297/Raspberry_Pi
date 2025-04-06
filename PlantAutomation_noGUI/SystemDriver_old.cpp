// SystemDriver.cpp
/*
        g++ -I/home/kpf5297/Code/ManualControl SystemDriver.cpp SystemController.cpp Logging.cpp LightController.cpp SoilSensor.cpp WaterPump.cpp ADS1115.cpp -o SystemDriver -lgpiod -lrt -lpthread

*/
#include "SystemController.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>


int16_t ADS1115_ADDRESS = 0x48;
int LIGHT_PIN = 27;
int PUMP_PIN = 17;
int LIGHT_WAIT_TIME = 45;
int PUMP_WAIT_TIME = 10;
int PUMP_DURATION = 5;
ADS1115::Mux MUX_SELECT = ADS1115::Mux::AIN0_GND;

int LIGHT_ON_DURATION = 5;

int main() {

    // Set daily on time for the current date
    time_t dailyOnTime = time(NULL);
    struct tm *onTime = localtime(&dailyOnTime);
    /*      Enable these if you want to also control a specific date      */
    // onTime->tm_mday = 25;
    // onTime->tm_mon = 11 - 1;
    // onTime->tm_year = 2023 - 1900;
    onTime->tm_hour = 15;
    onTime->tm_min = 47;
    onTime->tm_sec = 0;
    dailyOnTime = mktime(onTime);

    // Set daily off time for the current date
    time_t dailyOffTime = time(NULL);
    struct tm *offTime = localtime(&dailyOffTime);
    /*      Enable these if you want to also control a specific date      */
    // offTime->tm_mday = 25;
    // offTime->tm_mon = 11 - 1;
    // offTime->tm_year = 2023 - 1900;
    offTime->tm_hour = 15;
    offTime->tm_min = 47;
    offTime->tm_sec = 30;
    dailyOffTime = mktime(offTime);

    time_t currentTime = time(NULL);

    // Initialize system controller
    SystemController systemController(ADS1115_ADDRESS, MUX_SELECT, LIGHT_PIN, dailyOnTime, dailyOffTime, PUMP_PIN, PUMP_WAIT_TIME, PUMP_DURATION);

    // Update the pump end time to be 1 minute in the past so the pump starts with its wait duration and does not activate immediatly
    systemController.setWaterPumpOffTime(currentTime - PUMP_WAIT_TIME);

    // Update the light end time to be 1 minute in the past so the light starts with its wait duration and does not activate immediatly
    systemController.setLightOffTime(currentTime - LIGHT_ON_DURATION);

    // Calibrate the soil sensor
    systemController.calibrateSoilSensor();

    // Set the soil moisture threshold
    systemController.setSoilMoistureThreshold(50);

    time_t debug_Ligh_On_Time = time(NULL);
    time_t debug_Ligh_Off_Time = time(NULL);
    time_t debug_Next_Pump_Time = time(NULL);

    // Loop forever
    while (true) {

        // Update the current time
        currentTime = time(NULL);

        // cout current time
        std::cout << "Current time: " << ctime(&currentTime) << std::endl;

        // cout daily on time
        debug_Ligh_On_Time = systemController.getLightOnTime();
        std::cout << "Daily on time: " << ctime(&debug_Ligh_On_Time) << std::endl;

        // cout daily off time
        debug_Ligh_Off_Time = systemController.getLightOffTime();
        std::cout << "Daily off time: " << ctime(&debug_Ligh_Off_Time) << std::endl;

        // Activate the garden components
        systemController.controlLight(currentTime);

        // Wait 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Break once the current time is past the daily off time
        if (currentTime >  systemController.getLightOffTime()) {

            // Update ontime to be 45 seconds in the future from the current time
            systemController.setLightOnTime(currentTime + LIGHT_WAIT_TIME);

            // Update offtime to be 45 seconds in the future from the current time
            systemController.setLightOffTime(currentTime + LIGHT_ON_DURATION + LIGHT_WAIT_TIME);
        }

        // cout soil moisture and threshold
        std::cout << "Soil moisture: " << systemController.readSoilMoisture() << std::endl;
        std::cout << "Soil moisture threshold: " << systemController.getSoilMoistureThreshold() << std::endl;

        debug_Next_Pump_Time = systemController.getNextPumpTime();

        // cout next pump time
        std::cout << "Next pump time: " << ctime(&debug_Next_Pump_Time) << std::endl;

        // Activate the water components
        systemController.controlWaterPump(currentTime);
    }

    return 0;
}
