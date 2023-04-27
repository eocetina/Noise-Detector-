#include "SensorsServices.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WifiValues.h>
#include <DTOsServices.h>
#include <Battery.h>
#include <Humidity.h>
#include <Temperature.h>

using namespace SensorsServices;
extern MqttApi mqttApi;
/**
 * 
 **/
void SensorsServices::sendSensorsData(unsigned long currentWifiConnection, unsigned long lastWifiConnection, int wifiTimesLost, Sensors* sensors){  
  Hub *hub = Hub::getInstance();
  String timeOut = WifiValues::timeOut(currentWifiConnection, lastWifiConnection);
  int level = hub->soundLastValue();
  
  boolean publishOk = false;
  String data = DTOsServices::fromSensorsToJsonString(level, false, wifiTimesLost, timeOut, sensors);
  publishOk = mqttApi.publishNewSoundData(data); 
  if(!publishOk){
    DEBUG_PRINT("Imposible to publish sound message via MQTT");  
  }

  //DEBUG_PRINT(data);
  
}

/**
 * 
 **/
void SensorsServices::sendSensorsSingleAverage(unsigned long currentWifiConnection, unsigned long lastWifiConnection, int wifiTimesLost, Sensors* sensors){  
  
  Hub *hub = Hub::getInstance();
  DEBUG_PRINT("Publishing");
  String timeOut = WifiValues::timeOut(currentWifiConnection, lastWifiConnection);
  if (!hub->isAverageAvailable())
  {
    DEBUG_PRINT("Not average available");
    return;
  }
  int level = hub->soundAverage();
  
  boolean publishOk = false;
  String data = DTOsServices::fromSensorsToJsonString(level, true, wifiTimesLost, timeOut, sensors);
  publishOk = mqttApi.publishNewSoundData(data); 
  if(!publishOk){
    DEBUG_PRINT("Imposible to publish sound message via MQTT");  
  }
  //DEBUG_PRINT(data);
}

/**
 * 
 **/
void SensorsServices::sendSensorsArrayAverage(unsigned long currentWifiConnection, unsigned long lastWifiConnection, int wifiTimesLost, Sensors* sensors){  
  
  Hub *hub = Hub::getInstance();
  String timeOut = WifiValues::timeOut(currentWifiConnection, lastWifiConnection);
  while (!hub->isAverageAvailable());

  std::queue<int> sound_average;

  while(hub->isAverageAvailable())
  {
    sound_average.push(hub->soundAverage());
  }
  
  boolean publishOk = false;
  String data = DTOsServices::fromSensorsArrayToJsonString(sound_average, wifiTimesLost, timeOut, sensors);
  publishOk = mqttApi.publishNewSoundData(data);
  if(!publishOk){
    DEBUG_PRINT("Imposible to publish sound message via MQTT");  
  }
  //DEBUG_PRINT(data);
}
