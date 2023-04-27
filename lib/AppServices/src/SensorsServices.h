#ifndef SensorsServices_h
#define SensorsServices_h

#include <Arduino.h> //Import String class
#include <InOutApis.h>
#include <Hub.h>
#include <TimerHub.h>
#include <Sensors.h>



namespace SensorsServices{

  void sendSensorsData(unsigned long currentWifiConnection, unsigned long lastWifiConnection, int wifiTimesLost, Sensors* sensors);
  void sendSensorsSingleAverage(unsigned long currentWifiConnection, unsigned long lastWifiConnection, int wifiTimesLost, Sensors* sensors);
  void sendSensorsArrayAverage(unsigned long currentWifiConnection, unsigned long lastWifiConnection, int wifiTimesLost, Sensors* sensors);
  
}

#endif

