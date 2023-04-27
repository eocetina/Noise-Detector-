#ifndef HubServices_h
#define HubServices_h

#include <ArduinoJson.h>

namespace HubServices{
 
  bool requestSettings();
  void saveMqttSettings(JsonObject& root);
  void saveEcoSettings(JsonObject& root);
  void saveMicSettings(JsonObject& root);
  void requestStatus();
  void sendMqttConnectionError();
  bool updateFirmware(String firmwareUrl);
}

#endif

