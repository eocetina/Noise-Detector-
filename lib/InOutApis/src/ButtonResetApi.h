#ifndef ButtonResetApi_h
#define ButtonResetApi_h

#include <Arduino.h> //Import String class

class ButtonResetApi {
  
  public:
    ButtonResetApi(int pinNumber);
    void setup();
    unsigned long lastBtnPressMillis;
    void saveTimeNotPressed();
    boolean isPressedDuringTime(unsigned long miliseconds);
    boolean isPressed();
  
  private:
    int pinButton;
};

#endif