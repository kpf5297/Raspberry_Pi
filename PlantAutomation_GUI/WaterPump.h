#ifndef WATERPUMP_H
#define WATERPUMP_H

#include <gpiod.h>

/**
 * @brief The WaterPump class represents a water pump controlled by GPIO.
 */
class WaterPump {
public:
    /**
     * @brief Constructor for WaterPump.
     * @param pin GPIO pin number.
     * @param ignoreTimeSeconds The number of seconds to ignore the pump after activation.
     * @param pumpTimeSeconds The number of seconds to run the pump when activated.
     */
    WaterPump(int pin, int ignoreTimeSeconds, int pumpTimeSeconds);

    /**
     * @brief Destructor for WaterPump.
     */
    ~WaterPump();

    /**
     * @brief Activate the water pump.
     */
    void activate();

    /**
     * @brief Deactivate the water pump.
     */
    void deactivate();

    /**
     * @brief Toggle the water pump status.
     */
    void toggle();

    /**
     * @brief Get the water pump status.
     * @return True if the water pump is running, false otherwise.
     */
    bool getStatus();

    /**
     * @brief Set the water pump activation duration.
     * @param pumpTimeSeconds The duration to run the water pump when activated.
     */
    void setActivationDuration(int pumpTimeSeconds);

    /**
     * @brief Get the water pump activation duration.
     * @return The duration to run the water pump when activated.
     */
    int getActivationDuration();

    /**
     * @brief Set the time to ignore water pump activation after the last activation.
     * @param ignoreTimeSeconds The number of seconds to ignore the pump after activation.
     */
    void setIgnoreTime(int ignoreTimeSeconds);

    /**
     * @brief Get the time to ignore water pump activation after the last activation.
     * @return The number of seconds to ignore the pump after activation.
     */
    int getIgnoreTime();

private:
    gpiod_chip* chip;           // GPIO chip
    gpiod_line* line;           // GPIO line
    int pinNum;                 // GPIO pin number
    bool isRunning = false;     // Water pump status
    bool activated = false;     // Water pump activation status
    int activationDuration;     // Water pump activation duration
    int ignoreTime;             // Time to ignore water pump activation after last activation
};

#endif // WATERPUMP_H
