#ifndef Hub_h
#define Hub_h

#include <Arduino.h> //Import String class
//#include "Sound.h"
#include <deque>
#include <queue>
#include <numeric>

class Hub{
  private:
    uint8_t monitorId[6];
    String firmwareVersion;
    long lastSensorValue;
    boolean settingRequested;
    Hub();     
	  static Hub *instance;  
    static const unsigned int MAX_SOUND_SAMPLES = 12;
    std::deque<int>* sound;
    std::queue<int>* sound_average;
    static unsigned int eco_mode_interval;
    static unsigned int noise_adjustment;
 
  public:    
    static Hub *getInstance();
    void soundNewValue(int value);
    int soundAverage();
    int updateAverage();
    bool isAverageAvailable();
    int soundLastValue();
    void clearSoundAverageQueue();
    // Sound Management:
    //Sound sound = Sound();     
    
    String getFirmwareVersion();
    uint8_t* getMonitorId();
    String getMonitorIdString();
    String getSSIDAPName();    
    boolean isSettingsRequested();
    void settingsWasRequested();
    void resetSettings(); 

    static unsigned int getECOInterval();
    static void setECOInterval(unsigned int interval);

    static unsigned int getNoiseAdjustment();
    static void setNoiseAdjustment(unsigned int neg_offset);
};

#endif