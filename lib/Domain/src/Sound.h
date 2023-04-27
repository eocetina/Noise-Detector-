#ifndef Sound_h
#define Sound_h

#include <Arduino.h> //Import String class

class Sound{
  private:
    int levels[12];
    int currentIndex;
    
  
    
  public:      
    int lastValueSaved;
    
    Sound();   
    void saveNewValue(unsigned int newValue);
    int getAvarageLevels();
    boolean lastValueIsEndLevel();
    
};

#endif