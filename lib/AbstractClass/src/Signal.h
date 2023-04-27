#ifndef SIGNAL_H
#define SIGNAL_H

#include <list>
/**
 * @brief Abstract class to sample signal from a source
 * 
 */
class Signal
{
    protected:
    uint8_t samples;
    std::list <uint16_t> signal;
    uint32_t offset;

    public:
    virtual void updateSignal(uint16_t s_val) = 0;
    virtual int32_t readSignal() = 0;
};

#endif