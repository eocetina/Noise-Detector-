#ifndef TEMPERATURE_SI7021_H
#define TEMPERATURE_SI7021_H

#include "Temperature.h"
#include <Adafruit_Si7021.h>

class TemperatureSi7021 : public Temperature
{
    private:
    Adafruit_Si7021* sensor;
    public:
    TemperatureSi7021(Adafruit_Si7021* sensor);
    float getTemperature();
};

#endif