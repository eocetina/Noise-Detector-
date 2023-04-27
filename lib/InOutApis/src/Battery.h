#ifndef BATTERY_H
#define BATTERY_H

#include <inttypes.h>
#include "esp_adc_cal.h"

class Battery
{
    private:
    int pinBatteryLevel;
    int pinPowerIn;
    esp_adc_cal_characteristics_t *adc_chars;
    public:
    Battery(int pinPowerIn, int pinBatteryLevel);
    void setup();
    uint32_t readBatteryLevel();
    bool readPowerIn();
};
#endif