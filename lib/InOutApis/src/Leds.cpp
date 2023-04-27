#include "Leds.h"
#include <Arduino.h>

int Leds::blueLed = 0;   
int Leds::tickPin = 0;

/**
 * Constructor
 */ 
Leds::Leds()
{
  Leds::blueLed = 16;
  Leds::tickPin = 16;
}

/**
 * 
 */ 
void Leds::setup(){
  // BLUE LED
  pinMode(Leds::blueLed, OUTPUT);  
  allOff();
}

/**
 * 
 */ 
void Leds::allOff(){
  digitalWrite(Leds::blueLed, HIGH);
}

/**
 * 
 */ 
void Leds::startTesting(unsigned long miliseconds){  
  
  digitalWrite(Leds::blueLed, HIGH);
  delay(miliseconds);
  digitalWrite(Leds::blueLed, LOW);
  delay(miliseconds);
  digitalWrite(Leds::blueLed, HIGH);
  delay(miliseconds);
  digitalWrite(Leds::blueLed, LOW);
  delay(miliseconds);
  digitalWrite(Leds::blueLed, HIGH);
  delay(miliseconds);
  digitalWrite(Leds::blueLed, LOW);
  
}

void Leds::tick() {
  int state = digitalRead(Leds::tickPin);
  digitalWrite(Leds::tickPin, !state);
}

/**
 * 
 */ 
void Leds::blinkBlue(float elapse){
  Leds::tickPin = Leds::blueLed;
  ticker.attach(elapse, tick);
}

/**
 * 
 */ 
void Leds::blinkOff(){  
  allOff();
  ticker.detach();
}