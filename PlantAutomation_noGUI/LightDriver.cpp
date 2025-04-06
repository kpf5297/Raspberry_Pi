/**
 * @file LightDriver.cpp
 *
 * @brief A simple program to demonstrate the usage of the LightController class for controlling LEDs.
 *
 * Compile with:
 * g++ -I/home/kpf5297/Code/ManualControl LightDriver.cpp Logging.cpp LightController.cpp -o LightDriver -lgpiod -lrt -lpthread
 */

#include <unistd.h>
#include <iostream>
#include "Logging.h"
#include "LightController.h"

int BLUE_LED_PIN = 17;
int GREEN_LED_PIN = 27;

/**
 * @brief The main function of the program.
 * @return 0 on successful execution.
 */
int main() {
    // Set daily on time for the current date
    time_t dailyOnTime = time(NULL);
    struct tm* onTime = localtime(&dailyOnTime);
    /* Enable these if you want to also control a specific date */
    // onTime->tm_mday = 25;
    // onTime->tm_mon = 11 - 1;
    // onTime->tm_year = 2023 - 1900;
    onTime->tm_hour = 11;
    onTime->tm_min = 46;
    onTime->tm_sec = 0;
    dailyOnTime = mktime(onTime);

    // Set daily off time for the current date
    time_t dailyOffTime = time(NULL);
    struct tm* offTime = localtime(&dailyOffTime);
    /* Enable these if you want to also control a specific date */
    // offTime->tm_mday = 25;
    // offTime->tm_mon = 11 - 1;
    // offTime->tm_year = 2023 - 1900;
    offTime->tm_hour = 11;
    offTime->tm_min = 46;
    offTime->tm_sec = 30;
    dailyOffTime = mktime(offTime);

    // Create the light controllers
    LightController blueLED(BLUE_LED_PIN, dailyOnTime, dailyOffTime);
    LightController greenLED(GREEN_LED_PIN, dailyOnTime, dailyOffTime);

    // Get the current time
    time_t currentTime = time(NULL);

    // Turn on the lights
    while (true) {
        // Get the current time
        currentTime = time(NULL);

        // Turn on the lights if it's between the on and off times
        if (currentTime > blueLED.getDailyOnTime() && currentTime < blueLED.getDailyOffTime()) {
            // Turn on the lights
            blueLED.turnOn();
            greenLED.turnOn();
        } else {
            // Turn off the lights
            blueLED.turnOff();
            greenLED.turnOff();
        }

        // Sleep for 1 second
        sleep(1);

        // Break if the current time is greater than the off time
        if (currentTime > blueLED.getDailyOffTime()) {
            break;
        }
    }

    return 0;
}
