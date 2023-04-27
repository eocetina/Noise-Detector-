#ifndef TimerHub_h
#define TimerHub_h

#include <Arduino.h> //Import String class
#include <Ticker.h>
#include <HubTask.h>


//TODO delete hvac variables
//


class TimerHub{ 
  
  public:
    static const float UPDATE_SOUND_VALUE_SEC;     // 5sec
    static const float UPDATE_SOUND_AVERAGE_SEC;     // 60sec
    static const float NORMAL_MODE_SEND_SOUND_SEC;    //5 sec
    static const float NORMAL_MODE_SEND_AVERAGE_SEC;  //60 sec

    enum class Mode {NORMAL,ECO};

    

    TimerHub();  
    void setup();
    
    bool isTimeUpdateValues();
    bool isTimeSendValues();
    bool isTimeUpdateAverage();
    bool isTimeSendAverage();

    void valuesUpdated();
    void averageUpdated();
    void valuesSent();
    void averageSent();

    void changeToEcoMode();
    void changeToNormalMode();

    static Mode getMode();
  
  private:
    static Mode mode;
    //
    static bool updateSensorValues;
    static bool updateAverageValue;
    static bool sendSensorValues;
    static bool sendAverageValues;
    static unsigned int eco_count;
    //

    static void updateValuesCallback();
    static void updateAverageCallback();
    
    static Ticker valueUpdateTicker;
    static Ticker averageUpdateTicker;
};

#endif
