#ifndef TEMPERATURE_H
#define TEMPERATURE_H
/**
 * @brief Abstract class to get temperature
 * 
 */
class Temperature
{
    public:
    virtual float getTemperature() = 0;
};
#endif