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
    soilMoistureThreshold = 0.0;

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

    // grab the smallest unit of time possible and use it to create an ID for use in the logger
    time_t currentTime;
    time(&currentTime);
    std::string id = std::to_string(currentTime);
    
    // Log the creation
    logger.logEvent("INFO", "SystemController", "SystemController created with ID: " + id);
}

/**
 * @brief Destructor for SystemController.
 */
SystemController::~SystemController() {

    // log the destruction inlcuding the ID
    logger.logEvent("INFO", "SystemController" + id, "SystemController destroyed");  
    
}

/**
 * @brief Activate the garden components based on the current time.
 * @param currentTime Current time.
 */
void SystemController::activateGarden(const time_t currentTime) {
    //TBD
}

/**
 * @brief Deactivate the garden components based on the current time.
 * @param currentTime Current time.
 */
void SystemController::deactivateGarden(const time_t currentTime) {
    // TBD
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

        // Log the light activation
        logger.logEvent("INFO", "SystemController" + id, "Light activated");

    } else {
        lightController.turnOff();

        // Log the light deactivation
        logger.logEvent("INFO", "SystemController" + id, "Light deactivated");
    }


    // if (currentTime > lightOffTime) {
    //     lightOnTime = addSecondsToTime(currentTime, 86400);
    //     lightOffTime = addSecondsToTime(lightOnTime, 86400);

    //     // cout update time information
    //     std::cout << "UPDATED Light On Time: " << ctime(&lightOnTime) << std::endl;
    //     std::cout << "UPDATED Light Off Time: " << ctime(&lightOffTime) << std::endl;
    // }
}

/**
 * @brief Control the water pump based on the current time.
 * @details The water pump will be activated if the current time is within the water pump activation time and the soil
 *         moisture is below the threshold.
 * @param currentTime Current time.
 */
void SystemController::controlWaterPump(const time_t currentTime) {

    // Check if the current time is within the water pump activation time
    if (isTimeInRange(currentTime, waterPumpOnTime, waterPumpOffTime)) {
        // Check if the soil moisture is below the threshold
        if (readSoilMoisture() < soilMoistureThreshold) {
            waterPump.activate();

            // Log the water pump activation
            logger.logEvent("INFO", "SystemController" + id, "Water pump activated");
        }
    } else {
        waterPump.deactivate();

        // Log the water pump deactivation
        logger.logEvent("INFO", "SystemController" + id, "Water pump deactivated");
    }

    // Check if the current time is past the water pump off time
    if (currentTime > this->waterPumpOffTime) {
        this->waterPumpOnTime = addSecondsToTime(currentTime, this->pumpIgnoreTime);
        this->waterPumpOffTime = addSecondsToTime(this->waterPumpOnTime, this->pumpDuration);

        // Log the water pump time update
        logger.logEvent("INFO", "SystemController" + id, "Water pump time updated");
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

    // Log the soil sensor calibration
    logger.logEvent("INFO", "SystemController" + id, "Soil sensor calibration");

    return soilSensor.calibrate();
}

/**
 * @brief Set the soil moisture threshold.
 * @param threshold Threshold value.
 */
void SystemController::setSoilMoistureThreshold(double threshold) {

    // Log the soil moisture threshold update
    logger.logEvent("INFO", "SystemController" + id, "Soil moisture threshold updated from " +
                                                     std::to_string(threshold) + " to " +
                                                     std::to_string(soilMoistureThreshold));

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

    // Log the light on time update from to
    logger.logEvent("INFO", "SystemController" + id, "Light on time updated from " +
                                                     std::to_string(this->lightOnTime) + " to " +
                                                     std::to_string(lightOnTime));

    this->lightOnTime = lightOnTime;

}

/**
 * @brief Set the time to turn off the light after activation.
 * @param lightOffTime Time to turn off the light.
 */
void SystemController::setLightOffTime(const time_t lightOffTime) {

    // Log the light off time update from to
    logger.logEvent("INFO", "SystemController" + id, "Light off time updated from " +
                                                     std::to_string(this->lightOffTime) + " to " +
                                                     std::to_string(lightOffTime));

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

    // Log the water pump on time update from to
    logger.logEvent("INFO", "SystemController" + id, "Water pump on time updated from " +
                                                     std::to_string(this->waterPumpOnTime) + " to " +
                                                     std::to_string(waterPumpOnTime));

    this->waterPumpOnTime = waterPumpOnTime;
}

/**
 * @brief Set the time to turn off the water pump after activation.
 * @param waterPumpOffTime Time to turn off the water pump.
 */
void SystemController::setWaterPumpOffTime(const time_t waterPumpOffTime) {

    // Log the water pump off time update from to
    logger.logEvent("INFO", "SystemController" + id, "Water pump off time updated from " +
                                                     std::to_string(this->waterPumpOffTime) + " to " +
                                                     std::to_string(waterPumpOffTime));

    this->waterPumpOffTime = waterPumpOffTime;
}

/**
 * @brief Set the time to ignore water pump activation after the last activation.
 * @param pumpIgnoreTime Time to ignore the pump after activation.
 */
void SystemController::setPumpIgnoreTime(const time_t pumpIgnoreTime) {

    // Log the water pump ignore time update from to
    logger.logEvent("INFO", "SystemController" + id, "Water pump ignore time updated from " +
                                                     std::to_string(this->pumpIgnoreTime) + " to " +
                                                     std::to_string(pumpIgnoreTime));

    this->pumpIgnoreTime = pumpIgnoreTime;
}

/**
 * @brief Set the duration to run the water pump when activated.
 * @param pumpDuration Duration to run the water pump.
 */
void SystemController::setPumpDuration(int pumpDuration) {

    // Log the water pump duration update from to
    logger.logEvent("INFO", "SystemController" + id, "Water pump duration updated from " +
                                                     std::to_string(this->pumpDuration) + " to " +
                                                     std::to_string(pumpDuration));

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

/**
 * @brief Turn the light on or off.
 * @param on True to turn the light on, false to turn the light off.
 */
void SystemController::setLightOn(bool on) {

    if (on) {
        lightController.turnOn();

        // Log the light activation
        logger.logEvent("INFO", "SystemController" + id, "Light activated");

    } else
        lightController.turnOff();

    // Log the light deactivation
    logger.logEvent("INFO", "SystemController" + id, "Light deactivated");

}

/**
 * @brief Turn the water pump on or off.
 * @param on True to turn the water pump on, false to turn the water pump off.
 */
void SystemController::setWaterPumpOn(bool on) {
    if (on) {
        waterPump.activate();

        // Log the water pump activation
        logger.logEvent("INFO", "SystemController" + id, "Water pump activated");

    } else
        waterPump.deactivate();

    // Log the water pump deactivation
    logger.logEvent("INFO", "SystemController" + id, "Water pump deactivated");

}

/**
 * @brief Manualy set the soil moisture calibration values.
 * @param wetValue Calibration value for wet soil.
 * @param dryValue Calibration value for dry soil.
 */
void SystemController::setSoilMoistureCalibrationValues(int16_t wetValue, int16_t dryValue) {

    // Log the soil moisture calibration update from to
    logger.logEvent("INFO", "SystemController" + id, "Soil moisture calibration updated from " +
                                                     std::to_string(soilSensor.getWetCalValue()) + " to " +
                                                     std::to_string(wetValue));
    soilSensor.setWetCalValue(wetValue);

    // Log the soil moisture calibration update from to
    logger.logEvent("INFO", "SystemController" + id, "Soil moisture calibration updated from " +
                                                     std::to_string(soilSensor.getDryCalValue()) + " to " +
                                                     std::to_string(dryValue));
    soilSensor.setDryCalValue(dryValue);
}

