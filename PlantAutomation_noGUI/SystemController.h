// GardenController.h
#ifndef SYSTEMCONTROLLER_H
#define SYSTEMCONTROLLER_H

#include "WaterPump.h"
#include "LightController.h"
#include "SoilSensor.h"

class SystemController {
public:
    /**
     * @brief Constructor for GardenController.
     * @param soilSensorAddress Address of the soil sensor.
     * @param soilSensorMux Mux configuration for the soil sensor.
     * @param lightControllerPin GPIO pin number for the light controller.
     * @param lightOnTime Time to turn on light after activation.
     * @param lightOffTime Time to turn off light after activation.
     * @param waterPumpPin GPIO pin number for the water pump.
     * @param pumpIgnoreTimeSeconds Time to ignore the pump after activation.
     * @param pumpDurationSeconds Duration to run the pump when activated.
     */
    SystemController(uint8_t soilSensorAddress, ADS1115::Mux soilSensorMux,
                     int lightControllerPin, const time_t& lightOnTime, const time_t& lightOffTime,
                     int waterPumpPin, int pumpIgnoreTimeSeconds, int pumpDurationSeconds);

    /**
     * @brief Destructor for GardenController.
     */
    ~SystemController();

    /**
     * @brief Activate the garden components based on the current time.
     * @param currentTime Current time.
     */
    void activateGarden(const time_t currentTime);

    /**
     * @brief Deactivate the garden components based on the current time.
     * @param currentTime Current time.
     */
    void deactivateGarden(const time_t currentTime);

    /**
     * @brief Control the light based on the current time.
     * @param currentTime Current time.
     */
    void controlLight(const time_t currentTime);

    /**
     * @brief Control the water pump based on the current time.
     * @param currentTime Current time.
     */
    void controlWaterPump(const time_t currentTime);

    /**
     * @brief Read soil moisture from the soil sensor.
     * @return Soil moisture level.
     */
    double readSoilMoisture();

    /**
     * @brief Calibrate the soil sensor.
     * @return True if calibration is successful, false otherwise.
     */
    bool calibrateSoilSensor();

    /**
     * @brief Set the soil moisture threshold.
     * @param threshold Threshold value.
     */
    void setSoilMoistureThreshold(double threshold);

    /**
     * @brief Get the soil moisture threshold.
     * @return Soil moisture threshold.
     */
    double getSoilMoistureThreshold();

    /**
     * @brief Set the time to turn on the light after activation.
     * @param lightOnTime Time to turn on the light.
     */
    void setLightOnTime(const time_t lightOnTime);

    /**
     * @brief Set the time to turn off the light after activation.
     * @param lightOffTime Time to turn off the light.
     */
    void setLightOffTime(const time_t lightOffTime);

    /**
     * @brief Get the time to turn on the light after activation.
     * @return Time to turn on the light.
     */
    time_t getLightOnTime();

    /**
     * @brief Get the time to turn off the light after activation.
     * @return Time to turn off the light.
     */
    time_t getLightOffTime();

    /**
     * @brief Set the time to turn on the water pump after activation.
     * @param waterPumpOnTime Time to turn on the water pump.
     */
    void setWaterPumpOnTime(const time_t waterPumpOnTime);

    /**
     * @brief Set the time to turn off the water pump after activation.
     * @param waterPumpOffTime Time to turn off the water pump.
     */
    void setWaterPumpOffTime(const time_t waterPumpOffTime);

    /**
     * @brief Set the time to ignore water pump activation after the last activation.
     * @param pumpIgnoreTime Time to ignore the pump after activation.
     */
    void setPumpIgnoreTime(const time_t pumpIgnoreTime);

    /**
     * @brief Set the duration to run the water pump when activated.
     * @param pumpDuration Duration to run the water pump.
     */
    void setPumpDuration(int pumpDuration);

    /**
     * @brief Helper function to determine if the time is within range.
     * @param time Time to check.
     * @param start Start time.
     * @param end End time.
     * @return True if the time is within range, false otherwise.
     */
    bool isTimeInRange(const time_t& time, const time_t& start, const time_t& end);

    /**
     * @brief Helper function to add seconds to a time_t.
     * @param time Time to add seconds to.
     * @param seconds Seconds to add.
     * @return Time_t with seconds added.
     */
    time_t addSecondsToTime(const time_t& time, const int& seconds);

    /**
     * @brief Get the next time to activate the water pump.
     * @return Time to activate the water pump.
     */
    time_t getNextPumpTime();

private:
    SoilSensor soilSensor;          // Soil sensor controlled by the controller
    LightController lightController; // Light controller controlled by the controller
    WaterPump waterPump;            // Water pump controlled by the controller

    double soilMoistureThreshold;   // Soil moisture threshold
    time_t lightOnTime;              // Time to turn on the light
    time_t lightOffTime;             // Time to turn off the light
    time_t waterPumpOnTime;         // Time to turn on the water pump
    time_t waterPumpOffTime;        // Time to turn off the water pump
    int pumpIgnoreTime;          // Time to ignore water pump activation after last activation
    int pumpDuration;                // Water pump activation duration
};

#endif // GARDENCONTROLLER_H


