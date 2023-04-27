#include "Sound.h"

/**
 * 
*/
Sound::Sound(){
  currentIndex = 0;
  lastValueSaved=0;
}

/**
 * Calculate avarage level 
*/
int Sound::getAvarageLevels(){
  int level = 0;
  for (int i = 0; i < 11; i++) {
      level += levels[i];
  }
  level = level / 12;    
  return level;
}


/**
 * Check if the last value save is end level.
 */
void Sound::saveNewValue(unsigned int newValue){
  if(currentIndex>11){
    currentIndex = 0;
  }
  levels[currentIndex] = newValue;      
  currentIndex += 1;
  lastValueSaved = newValue;
}

/**
 * Check if the last value save is end level.
 */
boolean Sound::lastValueIsEndLevel(){
  return currentIndex>11;  
}