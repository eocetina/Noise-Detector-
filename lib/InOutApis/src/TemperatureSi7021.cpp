#include "TemperatureSi7021.h"
#include <Adafruit_Si7021.h>

TemperatureSi7021::TemperatureSi7021(Adafruit_Si7021* sensor)
{
    this->sensor = sensor;
}

float TemperatureSi7021::getTemperature()
{
    return sensor->readTemperature();
}