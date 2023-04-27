#ifndef Leds_h
#define Leds_h

#include <Arduino.h> //Import String class
#include <Ticker.h>

class Leds {
  
  public:
    Leds();
    void setup();
    void allOff();
    void startTesting(unsigned long miliseconds);
    void blinkBlue(float elapse);
    void blinkOff();
    
    static void tick();
    
  private:
    static int blueLed;   
    static int tickPin;
    Ticker ticker;    
};

#endif