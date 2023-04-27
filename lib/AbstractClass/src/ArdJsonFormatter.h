#ifndef ARDJSONFORMATTER_H
#define ARDJSONFORMATTER_H

#include <ArduinoJson.h>

class ArdJsonFormatter
{
    public:
    virtual JsonObject &format() = 0;
};

#endif