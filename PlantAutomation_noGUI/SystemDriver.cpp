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
int PUMP_WAIT_TIME = 30;
int PUMP_DURATION = 5;
int LIGHT_WAIT_TIME = 45;
int LIGHT_ON_DURATION = 5;

// Top Shelf
int TOP_LIGHT_PIN = 27;
int TOP_PUMP_PIN = 17;
ADS1115::Mux TS_MUX_SELECT = ADS1115::Mux::AIN0_GND;

// Bottom Shelf
int BOTTOM_LIGHT_PIN = 24;
int BOTTOM_PUMP_PIN = 23;
ADS1115::Mux BS_MUX_SELECT = ADS1115::Mux::AIN1_GND;

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
    SystemController topShelfControl(ADS1115_ADDRESS, TS_MUX_SELECT, TOP_LIGHT_PIN, dailyOnTime, dailyOffTime, TOP_PUMP_PIN, PUMP_WAIT_TIME, PUMP_DURATION);

    SystemController bottomShelfControl(ADS1115_ADDRESS, BS_MUX_SELECT, BOTTOM_LIGHT_PIN, dailyOnTime, dailyOffTime, BOTTOM_PUMP_PIN, PUMP_WAIT_TIME, PUMP_DURATION);

 
    topShelfControl.setWaterPumpOffTime(currentTime - PUMP_WAIT_TIME);      // Set the water pump off time to be 30 seconds in the past
    topShelfControl.setLightOffTime(currentTime - LIGHT_ON_DURATION);       // Set the light off time to be 5 seconds in the past
    std::cout << "Calibrating soil sensor on top shelf" << std::endl;       // Calibrate the soil sensor
    topShelfControl.calibrateSoilSensor();                                  // Calibrate the soil sensor
    topShelfControl.setSoilMoistureThreshold(50);                           // Set the soil moisture threshold to 50

    bottomShelfControl.setWaterPumpOffTime(currentTime - PUMP_WAIT_TIME);   // Set the water pump off time to be 30 seconds in the past
    bottomShelfControl.setLightOffTime(currentTime - LIGHT_ON_DURATION);    // Set the light off time to be 5 seconds in the past
    std::cout << "Calibrating soil sensor on bottom shelf" << std::endl;    // Calibrate the soil sensor
    bottomShelfControl.calibrateSoilSensor();                               // Calibrate the soil sensor
    bottomShelfControl.setSoilMoistureThreshold(50);                        // Set the soil moisture threshold to 50

    time_t TS_debug_Ligh_On_Time = time(NULL);
    time_t TS_debug_Ligh_Off_Time = time(NULL);
    time_t TS_debug_Next_Pump_Time = time(NULL);
    time_t BS_debug_Ligh_On_Time = time(NULL);
    time_t BS_debug_Ligh_Off_Time = time(NULL);
    time_t BS_debug_Next_Pump_Time = time(NULL);

    // Loop forever
    while (true) {

        // Update the current time
        currentTime = time(NULL);

        // cout current time
        std::cout << "Current time: " << ctime(&currentTime) << std::endl;

        // cout daily on time
        TS_debug_Ligh_On_Time = topShelfControl.getLightOnTime();
        std::cout << "Daily on time: " << ctime(&TS_debug_Ligh_On_Time) << std::endl;

        // cout daily off time
        TS_debug_Ligh_Off_Time = topShelfControl.getLightOffTime();
        std::cout << "Daily off time: " << ctime(&TS_debug_Ligh_Off_Time) << std::endl;

        // Activate the garden components
        topShelfControl.controlLight(currentTime);
        bottomShelfControl.controlLight(currentTime);

        // Wait 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Break once the current time is past the daily off time
        if (currentTime >  topShelfControl.getLightOffTime()) {

            // Update ontime to be 45 seconds in the future from the current time
            topShelfControl.setLightOnTime(currentTime + LIGHT_WAIT_TIME);

            // Update offtime to be 45 seconds in the future from the current time
            topShelfControl.setLightOffTime(currentTime + LIGHT_ON_DURATION + LIGHT_WAIT_TIME);
        }

        // Break once the current time is past the daily off time
        if (currentTime >  bottomShelfControl.getLightOffTime()) {

            // Update ontime to be 45 seconds in the future from the current time
            bottomShelfControl.setLightOnTime(currentTime + LIGHT_WAIT_TIME);

            // Update offtime to be 45 seconds in the future from the current time
            bottomShelfControl.setLightOffTime(currentTime + LIGHT_ON_DURATION + LIGHT_WAIT_TIME);
        }

        // cout soil moisture and threshold on top shelf
        std::cout << "Top Shelf Soil moisture: " << topShelfControl.readSoilMoisture() << std::endl;
        std::cout << "Top Shelf Soil moisture threshold: " << topShelfControl.getSoilMoistureThreshold() << std::endl;

        // cout soil moisture and threshold on bottom shelf
        std::cout << "Bottom Shelf Soil moisture: " << bottomShelfControl.readSoilMoisture() << std::endl;
        std::cout << "Bottom SHelf Soil moisture threshold: " << bottomShelfControl.getSoilMoistureThreshold() << std::endl;

        TS_debug_Next_Pump_Time = topShelfControl.getNextPumpTime();

        // cout next pump time
        std::cout << "Top Shelf Next pump time: " << ctime(&TS_debug_Next_Pump_Time) << std::endl;

        BS_debug_Next_Pump_Time = bottomShelfControl.getNextPumpTime();

        // cout next pump time
        std::cout << "Bottom Shelf Next pump time: " << ctime(&BS_debug_Next_Pump_Time) << std::endl;


        // Activate the water components
        topShelfControl.controlWaterPump(currentTime);
        bottomShelfControl.controlWaterPump(currentTime);
    }

    return 0;
}
