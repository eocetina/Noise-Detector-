#include "ButtonResetApi.h"


ButtonResetApi::ButtonResetApi(int pinNumber){    
  pinButton = pinNumber;
  lastBtnPressMillis = 0;
}

/**
 * 
 */ 
void  ButtonResetApi::setup(){
  // Define pin mode
  pinMode(pinButton, INPUT);
  digitalWrite(pinButton, HIGH);
}

/**
 * 
 */ 
void ButtonResetApi::saveTimeNotPressed(){
  if (digitalRead(pinButton) == HIGH) {    
    lastBtnPressMillis = millis();
  }
}

boolean ButtonResetApi::isPressed(){
  return digitalRead(pinButton) == LOW;
}
/**
 * 
 */ 
boolean ButtonResetApi::isPressedDuringTime(unsigned long miliseconds){
  if (millis() - lastBtnPressMillis > miliseconds) {
    return true;
  }
  return false;
}

