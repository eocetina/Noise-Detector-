#ifndef SENSORS_H
#define SENSORS_H

#include <Battery.h>
#include <Signal.h>
#include <Humidity.h>
#include <Temperature.h>
#include <memory>

class Sensors
{
    public:
    Battery* battery;
    Signal* microphone;
    Humidity* humidity;
    Temperature* temperature;

    Sensors(Battery* batt, Signal* micro, Humidity* hum, Temperature* temp);

    
};


#endif