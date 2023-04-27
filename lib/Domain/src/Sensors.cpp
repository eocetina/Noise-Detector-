#include "Sensors.h"

Sensors::Sensors(Battery* batt, Signal* micro, Humidity* hum, Temperature* temp)
{
    this->battery = batt;
    this->microphone = micro;
    this->humidity = hum;
    this->temperature = temp;
}