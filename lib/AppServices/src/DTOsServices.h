#ifndef DTOsServices_h
#define DTOsServices_h

#include <Arduino.h>     //Import String class
#include <MqttApi.h>
#include <Hub.h>
#include <Sensors.h>
#include <queue>
/**
 * Functions to tranforms Domamin object to Json String objects...
 */

namespace DTOsServices{
  //String fromEnvironmentToJsonString(Hub* hub);
  String fromSensorsToJsonString(int level, bool isEndlevel,int wifiTimesLost, String timeOut, Sensors* sensors);
  String fromSensorsArrayToJsonString (std::queue<int> &sound_average, int wifiTimesLost, String timeOut, Sensors* sensors);
  String fromMqttConnectionErrorToJsonString(MqttServer mqttServer);
  String fromHubToStatusJsonString(Hub* hub);
  String firmStatusJsonString(String msg);
}

#endif