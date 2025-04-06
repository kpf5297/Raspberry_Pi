#ifndef SOILSENSOR_H
#define SOILSENSOR_H

#include "ADS1115.h"

/**
 * @brief The SoilSensor class
 */
class SoilSensor {

public:
    /**
     * @brief Constructor for SoilSensor.
     * @param address Address of the soil sensor.
     * @param muxSelect Mux configuration for the soil sensor.
     */
    SoilSensor(uint8_t address, ADS1115::Mux muxSelect);

    /**
     * @brief Destructor for SoilSensor.
     */
    ~SoilSensor();

    /**
     * @brief Read the soil moisture.
     * @return Soil moisture level.
     */
    double readMoisture();

    /**
     * @brief Calibrate the soil sensor.
     * @return True if calibration was successful, false otherwise.
     */
    bool calibrate();

    /**
     * @brief Set the calibration values.
     * @param wetValue Calibration value for wet soil.
     */
    void setWetCalValue(int16_t wetValue);

    /**
     * @brief Set the calibration values.
     * @param wetValue Calibration value for wet soil.
     * @param sensor SoilSensor object.
     */
    static void setWetCalValue(int16_t wetValue, SoilSensor* sensor);

    /**
     * @brief Set the calibration values.
     * @param dryValue Calibration value for dry soil.
     */
    void setDryCalValue(int16_t dryValue);

    /**
     * @brief Set the calibration values.
     * @param dryValue Calibration value for dry soil.
     * @param sensor SoilSensor object.
     */
    static void setDryCalValue(int16_t dryValue, SoilSensor* sensor);

    /**
     * @brief Get the calibration values.
     * @return Calibration value for wet soil.
     */
    int16_t getWetCalValue();

    /**
     * @brief Get the calibration values.
     * @return Calibration value for dry soil.
     */
    int16_t getDryCalValue();

    /**
     * @brief Create the analog to digital object.
     * @return Analog to digital object.
     */
    ADS1115 ads1115; // Composition: SoilSensor has an ADS1115 object

private:

    ADS1115::Mux mux;                               // Mux configuration
    static const int16_t CAL_WET_DEFAULT = 32768;   // 2^15
    static const int16_t CAL_DRY_DEFAULT = 0;       // 0
    double moisture;                                // Moisture level
    int16_t calWetValue;                            // Calibration value for wet soil
    int16_t calDryValue;                            // Calibration value for dry soil


    /**
     * @brief Map the value from one range to another.
     * @param x Value to map.
     * @param in_min Minimum value of the input range.
     * @param in_max Maximum value of the input range.
     * @param out_min Minimum value of the output range.
     * @param out_max Maximum value of the output range.
     * @return Mapped value.
     */
    double map(double x, double in_min, double in_max, double out_min, double out_max);

    /**
     * @brief Constrain the value to a range.
     * @param x Value to constrain.
     * @param min Minimum value of the range.
     * @param max Maximum value of the range.
     * @return Constrained value.
     */
    double constrain(double x, double min, double max);
};

#endif // SOILSENSOR_H

