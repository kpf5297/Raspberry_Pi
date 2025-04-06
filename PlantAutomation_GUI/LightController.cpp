#include "LightController.h"

/**
 * @brief Constructor for LightController.
 * @param pin GPIO pin number.
 * @param dailyOn Time to turn on light after activation.
 * @param dailyOff Time to turn off light after activation.
 */
LightController::LightController(int pin, const time_t& dailyOn, const time_t& dailyOff) {
    // Set GPIO pin number
    pinNum = pin;

    // Set daily on and off times
    dailyOnTime = dailyOn;
    dailyOffTime = dailyOff;

    // Set light status
    on = false;

    // Open GPIO chip
    chip = gpiod_chip_open("/dev/gpiochip0");

    // Request GPIO line
    line = gpiod_chip_get_line(chip, pinNum);

    // Set GPIO line direction to output
    gpiod_line_request_output(line, "LightController", 0);

    // Turn off light
    gpiod_line_set_value(line, 0);
}

/**
 * @brief Overloaded Constructor for LightController.
 * @param pin GPIO pin number.
 */
LightController::LightController(int pin) {
    // Set GPIO pin number
    pinNum = pin;

    // Set light status
    on = false;

    // Open GPIO chip
    chip = gpiod_chip_open("/dev/gpiochip0");

    // Request GPIO line
    line = gpiod_chip_get_line(chip, pinNum);

    // Set GPIO line direction to output
    gpiod_line_request_output(line, "LightController", 0);

    // Turn off light
    gpiod_line_set_value(line, 0);
}

/**
 * @brief Destructor for LightController.
 */
LightController::~LightController() {
    // Close GPIO chip
    gpiod_chip_close(chip);

    // Free GPIO line
    gpiod_line_release(line);
}

/**
 * @brief Turn on light.
 */
void LightController::turnOn() {
    // Turn on light
    gpiod_line_set_value(line, 1);

    // Set light status
    on = true;
}

/**
 * @brief Turn off light.
 */
void LightController::turnOff() {
    // Turn off light
    gpiod_line_set_value(line, 0);

    // Set light status
    on = false;
}

/**
 * @brief Toggle light status.
 */
void LightController::toggle() {
    // Toggle light status
    if (on) {
        turnOff();
    } else {
        turnOn();
    }

    // Set light status
    on = !on;
}

/**
 * @brief Get light status.
 * @return True if the light is on, false otherwise.
 */
bool LightController::isOn() {
    return on;
}

/**
 * @brief Set time to turn on light after activation.
 * @param time Time to turn on light after activation.
 */
void LightController::setDailyOn(const time_t time) {
    dailyOnTime = time;
}

/**
 * @brief Set time to turn off light after activation.
 * @param time Time to turn off light after activation.
 */
void LightController::setDailyOff(const time_t time) {
    dailyOffTime = time;
}

/**
 * @brief Get time to turn on light after activation.
 * @return Time to turn on light.
 */
time_t LightController::getDailyOnTime() {
    return dailyOnTime;
}

/**
 * @brief Get time to turn off light after activation.
 * @return Time to turn off light.
 */
time_t LightController::getDailyOffTime() {
    return dailyOffTime;
}
