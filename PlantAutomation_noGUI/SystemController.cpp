// SystemController.cpp
#include "SystemController.h"

/**
 * @file SystemController.cpp
 *
 * @brief Implementation of the SystemController class.
 */

/**
 * @brief Constructor for SystemController.
 * @param soilSensorAddress Address of the soil sensor.
 * @param soilSensorMux Mux configuration for the soil sensor.
 * @param lightControllerPin GPIO pin number for the light controller.
 * @param lightOnTime Time to turn on light after activation.
 * @param lightOffTime Time to turn off light after activation.
 * @param waterPumpPin GPIO pin number for the water pump.
 * @param pumpIgnoreTimeSeconds Time to ignore the pump after activation.
 * @param pumpDurationSeconds Duration to run the pump when activated.
 */
SystemController::SystemController(uint8_t soilSensorAddress, ADS1115::Mux soilSensorMux,
                                   int lightControllerPin, const time_t& lightOnTime, const time_t& lightOffTime,
                                   int waterPumpPin, int pumpIgnoreTimeSeconds, int pumpDurationSeconds) :
        soilSensor(soilSensorAddress, soilSensorMux),
        lightController(lightControllerPin, lightOnTime, lightOffTime),
        waterPump(waterPumpPin, pumpIgnoreTimeSeconds, pumpDurationSeconds) {
    // Set the initial soil moisture threshold
    soilMoistureThreshold = 0.5;

    // Set the initial light on and off times
    this->lightOnTime = lightOnTime;
    this->lightOffTime = lightOffTime;

    // Set the initial water pump on and off times
    waterPumpOnTime = lightOnTime;
    waterPumpOffTime = lightOffTime;

    // Set the initial water pump ignore time
    pumpIgnoreTime = pumpIgnoreTimeSeconds;

    // Set the initial water pump activation duration
    pumpDuration = pumpDurationSeconds;
}

/**
 * @brief Destructor for SystemController.
 */
SystemController::~SystemController() {
    
}

/**
 * @brief Activate the garden components based on the current time.
 * @param currentTime Current time.
 */
void SystemController::activateGarden(const time_t currentTime) {
    // Activate the light controller
    lightController.turnOn();

    // Activate the water pump if the soil moisture is below the threshold
    if (readSoilMoisture() < soilMoistureThreshold) {
        waterPump.activate();
    }
}

/**
 * @brief Deactivate the garden components based on the current time.
 * @param currentTime Current time.
 */
void SystemController::deactivateGarden(const time_t currentTime) {
    // Deactivate the light controller
    lightController.turnOff();

    // Deactivate the water pump
    waterPump.deactivate();
}

/**
 * @brief Control the light based on the current time.
 * @details The light will be activated if the current time is within the light activation time.
 * @param currentTime Current time.
 */
void SystemController::controlLight(const time_t currentTime) {
    // Check if the current time is within the light activation time
    if (isTimeInRange(currentTime, lightOnTime, lightOffTime)) {
        lightController.turnOn();
    } else {
        lightController.turnOff();
    }
}

/**
 * @brief Control the water pump based on the current time.
 * @details The water pump will be activated if the current time is within the water pump activation time and the soil
 *         moisture is below the threshold.
 * @param currentTime Current time.
 */
void SystemController::controlWaterPump(const time_t currentTime) {
    // Check if the current time is within the water pump activation time
    std::cout << "In Function controlWaterPump" << std::endl;
    std::cout << "Current Time: " << ctime(&currentTime) << std::endl;
    std::cout << "Water Pump On Time: " << ctime(&waterPumpOnTime) << std::endl;
    std::cout << "Water Pump Off Time: " << ctime(&waterPumpOffTime) << std::endl;

    if (isTimeInRange(currentTime, waterPumpOnTime, waterPumpOffTime)) {
        // Check if the soil moisture is below the threshold
        if (readSoilMoisture() < soilMoistureThreshold) {
            waterPump.activate();

            // Update the water pump with wait time
            this->waterPumpOnTime = addSecondsToTime(currentTime, this->pumpIgnoreTime);
            this->waterPumpOffTime = addSecondsToTime(this->waterPumpOnTime, this->pumpDuration);

                        // cout update time information
            std::cout << "UPDATED Water Pump On Time: " << ctime(&waterPumpOnTime) << std::endl;
            std::cout << "UPDATED Water Pump Off Time: " << ctime(&waterPumpOffTime) << std::endl;
        }
    } else {
        waterPump.deactivate();
    }

    if (currentTime > this->waterPumpOffTime) {
        this->waterPumpOnTime = addSecondsToTime(currentTime, this->pumpIgnoreTime);
        this->waterPumpOffTime = addSecondsToTime(this->waterPumpOnTime, this->pumpDuration);
    }
}

/**
 * @brief Read soil moisture from the soil sensor.
 * @return Soil moisture level.
 */
double SystemController::readSoilMoisture() {
    return soilSensor.readMoisture();
}

/**
 * @brief Calibrate the soil sensor.
 * @return True if calibration is successful, false otherwise.
 */
bool SystemController::calibrateSoilSensor() {
    return soilSensor.calibrate();
}

/**
 * @brief Set the soil moisture threshold.
 * @param threshold Threshold value.
 */
void SystemController::setSoilMoistureThreshold(double threshold) {
    soilMoistureThreshold = threshold;
}

/**
 * @brief Get the soil moisture threshold.
 * @return Soil moisture threshold.
 */
double SystemController::getSoilMoistureThreshold() {
    return soilMoistureThreshold;
}

/**
 * @brief Set the time to turn on the light after activation.
 * @param lightOnTime Time to turn on the light.
 */
void SystemController::setLightOnTime(const time_t lightOnTime) {
    this->lightOnTime = lightOnTime;
}

/**
 * @brief Set the time to turn off the light after activation.
 * @param lightOffTime Time to turn off the light.
 */
void SystemController::setLightOffTime(const time_t lightOffTime) {
    this->lightOffTime = lightOffTime;
}

/**
 * @brief Get the time to turn on the light after activation.
 * @return Time to turn on the light.
 */
time_t SystemController::getLightOnTime() {
    return lightOnTime;
}

/**
 * @brief Get the time to turn off the light after activation.
 * @return Time to turn off the light.
 */
time_t SystemController::getLightOffTime() {
    return lightOffTime;
}

/**
 * @brief Set the time to turn on the water pump after activation.
 * @param waterPumpOnTime Time to turn on the water pump.
 */
void SystemController::setWaterPumpOnTime(const time_t waterPumpOnTime) {
    this->waterPumpOnTime = waterPumpOnTime;
}

/**
 * @brief Set the time to turn off the water pump after activation.
 * @param waterPumpOffTime Time to turn off the water pump.
 */
void SystemController::setWaterPumpOffTime(const time_t waterPumpOffTime) {
    this->waterPumpOffTime = waterPumpOffTime;
}

/**
 * @brief Set the time to ignore water pump activation after the last activation.
 * @param pumpIgnoreTime Time to ignore the pump after activation.
 */
void SystemController::setPumpIgnoreTime(const time_t pumpIgnoreTime) {
    this->pumpIgnoreTime = pumpIgnoreTime;
}

/**
 * @brief Set the duration to run the water pump when activated.
 * @param pumpDuration Duration to run the water pump.
 */
void SystemController::setPumpDuration(int pumpDuration) {
    this->pumpDuration = pumpDuration;
}

/**
 * @brief Helper function to determine if the time is within range.
 * @param time Time to check.
 * @param start Start time.
 * @param end End time.
 * @return True if the time is within range, false otherwise.
 */
bool SystemController::isTimeInRange(const time_t& time, const time_t& start, const time_t& end) {
    return (difftime(time, start) >= 0) && (difftime(end, time) >= 0);
}

/**
 * @brief Helper function to add seconds to a time_t.
 * @param time Time to add seconds to.
 * @param seconds Seconds to add.
 * @return Time_t with seconds added.
 */
time_t SystemController::addSecondsToTime(const time_t& time, const int& seconds) {
    return time + seconds;
}

/**
 * @brief Get the next time to activate the water pump.
 * @return Next time to activate the water pump.
 */
time_t SystemController::getNextPumpTime() {
    return addSecondsToTime(waterPumpOffTime, pumpIgnoreTime);
}

