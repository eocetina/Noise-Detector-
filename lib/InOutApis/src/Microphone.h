#ifndef MICROPHONE_H
#define MICROPHONE_H

#include "Signal.h"

class Microphone : public Signal
{
    private:
    int pinMicro;
    public:
    Microphone(int pinNumber);
    void setup();
    void updateSignal(uint16_t s_val);
    int32_t readSignal();
    void updateOffset();
    void updateAllSignal();
    uint32_t getZCR();
    bool changeResistance(int res);
};

#endif