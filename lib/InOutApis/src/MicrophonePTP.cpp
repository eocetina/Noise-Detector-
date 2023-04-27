#include "MicrophonePTP.h"
#include <Arduino.h>

MicrophonePTP::MicrophonePTP(int pinNumber)
{
    this->pinMicro = pinNumber;
}

void MicrophonePTP::setup()
{
    // Defie pin mode
    pinMode(pinMicro, INPUT);
    // RESISTANCE SWITCH
    pinMode(17, OUTPUT);
    pinMode(18, OUTPUT);
    pinMode(19, OUTPUT);
    pinMode(15, OUTPUT);


    // INITIAL RESISTANCE SWITCH TO 3
    digitalWrite(17, LOW);
    digitalWrite(18, LOW);
    digitalWrite(19, HIGH);
    digitalWrite(15, LOW);

    //wait 10 ms from the resistance configuration
    delay(10);

    samples = 50;
    offset = 0;

}

int32_t MicrophonePTP::readSignal()
{
    unsigned long startTime = millis();
    unsigned int PTPAmp = 0;
    unsigned int maxAmp = 0;
    unsigned int minAmp = 4095;
    int micOut;
    while (millis() - startTime < samples)
    {
        micOut = analogRead(pinMicro);
        if ( micOut < 4095)
        {
        if (micOut > maxAmp)
        {
            maxAmp = micOut;
        }
        else if (micOut < minAmp)
        {
            minAmp = micOut;
        }
        }
    }
    PTPAmp = maxAmp - minAmp;
    double micOut_Volts = (PTPAmp * 3.3) / 4096;

    return PTPAmp;
}

bool MicrophonePTP::changeResistance(int res)
{
    int gpio;
    switch (res) {
        case 1:
        gpio = 17;
        break;
        case 2:
        gpio = 18;
        break;
        case 3:
        gpio = 19;
        break;
        case 4:
        gpio = 15;
        break;
    }

    if (digitalRead(gpio) == HIGH) {
        return false;
    }
    
    digitalWrite(gpio, HIGH);
    delay(1);

    if (digitalRead(17) != LOW and gpio != 17) {
        digitalWrite(17, LOW);
    }
    
    if (digitalRead(18) != LOW and gpio != 18) {
        digitalWrite(18, LOW);
    }
    
    if (digitalRead(19) != LOW and gpio != 19) {
        digitalWrite(19, LOW);
    }
    
    if (digitalRead(15) != LOW and gpio != 15) {
        digitalWrite(15, LOW);
    }
    
    delay(1);

    return true;
}