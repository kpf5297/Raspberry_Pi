#include "WaterPump.h"

/**
 * @file WaterPump.cpp
 *
 * @brief Implementation of the WaterPump class.
 */

/**
 * @brief Constructor for WaterPump.
 * @param pin GPIO pin number.
 * @param ignoreTimeSeconds Time to ignore water pump activation after the last activation.
 * @param pumpTimeSeconds Time to run the water pump.
 */
WaterPump::WaterPump(int pin, int ignoreTimeSeconds, int pumpTimeSeconds) {
    // Store the pin number
    pinNum = pin;

    // Store the activation duration
    activationDuration = pumpTimeSeconds;

    // Store the ignore time
    ignoreTime = ignoreTimeSeconds;

    // Open the GPIO chip
    chip = gpiod_chip_open("/dev/gpiochip0");

    // Get the GPIO line
    line = gpiod_chip_get_line(chip, pinNum);

    // Configure the GPIO line as an output
    gpiod_line_request_output(line, "WaterPump", 0);

    // Set the initial state of the GPIO line to low
    gpiod_line_set_value(line, 0);
}

/**
 * @brief Destructor for WaterPump.
 */
WaterPump::~WaterPump() {
    gpiod_line_release(line);
    gpiod_chip_close(chip);
}

/**
 * @brief Turn on the water pump.
 */
void WaterPump::activate() {
    gpiod_line_set_value(line, 1);
    isRunning = true;
}

/**
 * @brief Turn off the water pump.
 */
void WaterPump::deactivate() {
    gpiod_line_set_value(line, 0);
    isRunning = false;
}

/**
 * @brief Toggle the water pump status.
 */
void WaterPump::toggle() {
    if (isRunning) {
        deactivate();
    } else {
        activate();
    }
}

/**
 * @brief Get the water pump status.
 * @return True if the water pump is running, false otherwise.
 */
bool WaterPump::getStatus() {
    return isRunning;
}

/**
 * @brief Set the water pump activation duration.
 * @param pumpTimeSeconds Time to run the water pump.
 */
void WaterPump::setActivationDuration(int pumpTimeSeconds) {
    activationDuration = pumpTimeSeconds;
}

/**
 * @brief Get the water pump activation duration.
 * @return The duration to run the water pump.
 */
int WaterPump::getActivationDuration() {
    return activationDuration;
}

/**
 * @brief Set the time to ignore water pump activation after the last activation.
 * @param ignoreTimeSeconds Time to ignore water pump activation after the last activation.
 */
void WaterPump::setIgnoreTime(int ignoreTimeSeconds) {
    ignoreTime = ignoreTimeSeconds;
}

/**
 * @brief Get the time to ignore water pump activation after the last activation.
 * @return Time to ignore water pump activation after the last activation.
 */
int WaterPump::getIgnoreTime() {
    return ignoreTime;
}
