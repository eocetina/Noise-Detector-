#ifndef HUMIDITY_SI7021_H
#define HUMIDITY_SI7021_H

#include "Humidity.h"
#include <Adafruit_Si7021.h>


class HumiditySi7021 : public Humidity
{
    private:
    Adafruit_Si7021* sensor;
    public:
    HumiditySi7021(Adafruit_Si7021* sensor);
    float getHumidity();
};

#endif