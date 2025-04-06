#ifndef SOILSENSOR_H
#define SOILSENSOR_H

#include "ADS1115.h"

class SoilSensor {
public:
    SoilSensor(uint8_t address, ADS1115::Mux muxSelect);
    ~SoilSensor();

    double readMoisture();
    bool calibrate();
    void setWetCalValue(int16_t wetValue);
    void setDryCalValue(int16_t dryValue);
    int16_t getWetCalValue();
    int16_t getDryCalValue();

    ADS1115 ads1115; // Composition: SoilSensor has an ADS1115 object

private:

    ADS1115::Mux mux;
    
    // Calibration constants
    static const int16_t CAL_WET_DEFAULT = 32768;   // 2^15
    static const int16_t CAL_DRY_DEFAULT = 0;

    double moisture;
    int16_t calWetValue;
    int16_t calDryValue;

    // Private helper functions
    double map(double x, double in_min, double in_max, double out_min, double out_max);
    double constrain(double x, double min, double max);
};

#endif // SOILSENSOR_H

