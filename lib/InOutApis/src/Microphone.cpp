#include "Microphone.h"
#include <Arduino.h>

Microphone::Microphone(int pinNumber)
{
    this->pinMicro = pinNumber;
}

void Microphone::setup()
{
    // Defie pin mode
    pinMode(pinMicro, INPUT);
    // Set ADC to 10 bits
    analogSetWidth(10);
    // RESISTANCE SWITCH
    pinMode(17, OUTPUT);
    pinMode(18, OUTPUT);
    pinMode(19, OUTPUT);
    pinMode(23, OUTPUT);


    // INITIAL RESISTANCE SWITCH TO 3
    digitalWrite(17, LOW);
    digitalWrite(18, LOW);
    digitalWrite(19, HIGH);
    digitalWrite(23, LOW);

    //wait 10 ms from the resistance configuration
    delay(10);

    samples = 100;
    offset = 0;
    uint16_t s_val;
     for (unsigned int i = 0; i < samples; i++)
    {
        s_val = analogRead(pinMicro);
        signal.push_back(s_val);
        offset += s_val;
        delay(1);
    }

    offset /= 100;

}

void Microphone::updateSignal(uint16_t s_val)
{
    signal.pop_front();
    signal.push_back(s_val);
}

void Microphone::updateOffset()
{
    offset = 0;

    uint16_t s_val;
    for (unsigned int i = 0; i < samples; i++)
    {
        s_val = analogRead(pinMicro);
        signal.pop_front();
        signal.push_back(s_val);
        offset += s_val;
        delay(1);
        
    }

    offset /= samples;

}

void Microphone::updateAllSignal()
{
    int16_t s_val;
    for (unsigned int i = 0; i < samples; i++)
    {
        s_val = analogRead(pinMicro);
        signal.pop_front();
        signal.push_back(s_val);
        delay(1);
    }

}

uint32_t Microphone::getZCR()
{
    std :: list <uint16_t> :: iterator it = signal.begin();
    uint32_t zcr = 0;
    int32_t zcr_sub = (int32_t)(*it - offset);
    ++it;
    
    for (; it != signal.end(); ++it)
    {
        //zcr += abs(sign(*it - offset)-sign(zcr_sub));
        zcr += abs(zcr_sub - (int32_t)(*it - offset));
        zcr_sub = (int32_t)(*it - offset);
        zcr=zcr/4;
    }
    return zcr;
}

int32_t Microphone::readSignal()
{
    updateAllSignal();
    int i = 0;
    uint32_t adc = 0;

    while (i < 100) {
        updateSignal(analogRead(pinMicro));
        adc += getZCR();
        //delay(1);
        i++;
    }

    yield();

    adc = adc / 100;
    if (adc > 1023){
        adc = 1023;
    }
    
    return (int)adc;
}


bool Microphone::changeResistance(int res)
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
        gpio = 23;
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
    
    if (digitalRead(23) != LOW and gpio != 15) {
        digitalWrite(23, LOW);
    }
    
    delay(1);
    
    updateOffset();

    return true;
}