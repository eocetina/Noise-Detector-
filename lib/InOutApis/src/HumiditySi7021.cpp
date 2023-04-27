#include "HumiditySi7021.h"
#include <Adafruit_Si7021.h>

HumiditySi7021::HumiditySi7021(Adafruit_Si7021* sensor)
{
    this->sensor = sensor;
}

float HumiditySi7021::getHumidity()
{
    return sensor->readHumidity();
}