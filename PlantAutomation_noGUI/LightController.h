// LightController.h
#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#include <gpiod.h>
#include <ctime>
#include <iostream>

class LightController {
public:
    /**
     * @brief Constructor for LightController.
     * @param pin GPIO pin number.
     * @param dailyOn Time to turn on light after activation.
     * @param dailyOff Time to turn off light after activation.
     */
    LightController(int pin, const time_t& dailyOn, const time_t& dailyOff);

    /**
     * @brief Overloaded Constructor for LightController.
     * @param pin GPIO pin number.
     */
    LightController(int pin);

    /**
     * @brief Destructor for LightController.
     */
    ~LightController();

    /**
     * @brief Turn on the light.
     */
    void turnOn();

    /**
     * @brief Turn off the light.
     */
    void turnOff();

    /**
     * @brief Toggle the light status.
     */
    void toggle();

    /**
     * @brief Get the light status.
     * @return True if the light is on, false otherwise.
     */
    bool isOn();

    /**
     * @brief Set the time to turn on the light after activation.
     * @param time Time to turn on the light.
     */
    void setDailyOn(const time_t time);

    /**
     * @brief Set the time to turn off the light after activation.
     * @param time Time to turn off the light.
     */
    void setDailyOff(const time_t time);

    /**
     * @brief Get the time to turn on the light after activation.
     * @return Time to turn on the light.
     */
    time_t getDailyOnTime();

    /**
     * @brief Get the time to turn off the light after activation.
     * @return Time to turn off the light.
     */
    time_t getDailyOffTime();

    /**
     * @brief Overloaded operator for debugging.
     * @param os Output stream.
     * @param lc LightController object to output.
     * @return Reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const LightController& lc);

private:
    gpiod_chip *chip;       // GPIO chip
    gpiod_line *line;       // GPIO line
    int pinNum;             // GPIO pin number
    bool on;                // Light status
    time_t dailyOnTime;     // Time to turn on light after activation
    time_t dailyOffTime;    // Time to turn off light after activation
};

#endif // LIGHTCONTROLLER_H

