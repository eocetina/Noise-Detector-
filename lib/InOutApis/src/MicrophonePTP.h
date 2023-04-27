#ifndef MICROPHONEPTP_H
#define MICROPHONEPTP_H

#include "Signal.h"

class MicrophonePTP : public Signal
{
    private:
    int pinMicro;
    public:
    MicrophonePTP(int pinNumber);
    void setup();
    int32_t readSignal();
    bool changeResistance(int res);
    void updateSignal(uint16_t s_val) //not used
    {
        
    }
};

#endif