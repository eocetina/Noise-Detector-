#include "DTOsServices.h"
#include <ArduinoJson.h>
#include <WifiValues.h>
#include <esp_system.h>
#include <Battery.h>

using namespace DTOsServices;

/**
 *
*/
String DTOsServices::fromSensorsToJsonString(int level, bool isEndlevel,
  int wifiTimesLost, String timeOut, Sensors* sensors){

  String data;  
  DynamicJsonBuffer jBuffer;
  JsonObject& root = jBuffer.createObject();
  //
  if(isEndlevel){
    root["endlevel"] = String(level);
  }else{
    root["level"] = String(level);
  }
  
  root["ssid"] = WiFi.SSID();
  root["wifi"] = WifiValues::quality();
  root["wifilost"] = String(wifiTimesLost);
  root["localip"] = WifiValues::localIp();
  root["alive"] = String(millis() / 1000);
  root["timeout"] = timeOut; 
  root["battery_level"] = String(sensors->battery->readBatteryLevel());
  root["battery_mode"] = String(!sensors->battery->readPowerIn());
  root["humidity"] = String(sensors->humidity->getHumidity());
  root["temperature"] = String(sensors->temperature->getTemperature());

  root.printTo(data);
  return data;
  
}

String DTOsServices::fromSensorsArrayToJsonString(std::queue<int>& sound_average,
int wifiTimesLost, String timeOut, Sensors* sensors)
{

  String data;  
  DynamicJsonBuffer jBuffer;
  JsonObject& root = jBuffer.createObject();
  JsonArray& average_array = root.createNestedArray("endlevel_array");
  //
  while(!sound_average.empty())
  {
    average_array.add(sound_average.front());
    sound_average.pop();
  }
  root["ssid"] = WiFi.SSID();
  root["wifi"] = WifiValues::quality();
  root["wifilost"] = String(wifiTimesLost);
  root["localip"] = WifiValues::localIp();
  root["alive"] = String(millis() / 1000);
  root["timeout"] = timeOut; 
  root["battery_level"] = String(sensors->battery->readBatteryLevel());
  root["battery_mode"] = String(!sensors->battery->readPowerIn());
  root["humidity"] = String(sensors->humidity->getHumidity());
  root["temperature"] = String(sensors->temperature->getTemperature());

  root.printTo(data);
  return data;
  
}


/**
 * 
*/
String DTOsServices::fromMqttConnectionErrorToJsonString(MqttServer mqttServer){
  String errorJson;
  DynamicJsonBuffer jBuffer;
  JsonObject& root = jBuffer.createObject();
  
  // Firmware version.
  root["type"]  = "mqtt";  
  root["host"] = mqttServer.host;
  root["port"] = mqttServer.port;
  root["user"] = mqttServer.user;  
  root.printTo(errorJson);  
  return errorJson;
}

/*!
 */

String getResetReason(esp_reset_reason_t reason)
{
  char buff[32];

  switch (reason)
  {
    case ESP_RST_UNKNOWN:
      strcpy_P(buff, PSTR("Unknown"));
      break;
    case ESP_RST_POWERON:
      strcpy_P(buff, PSTR("Power on"));
      break;
    case ESP_RST_EXT:
      strcpy_P(buff, PSTR("External System"));
      break;
    case ESP_RST_SW:
      strcpy_P(buff, PSTR("Software/System restart"));
      break;
    case ESP_RST_PANIC:
      strcpy_P(buff, PSTR("Exception/Panic"));
      break;
    case ESP_RST_INT_WDT:
      strcpy_P(buff, PSTR("Interrupt watchdog"));
      break;
    case ESP_RST_TASK_WDT:
      strcpy_P(buff, PSTR("Task watchdog"));
      break;
    case ESP_RST_WDT:
      strcpy_P(buff, PSTR("Other watchdogs"));
      break;
    case ESP_RST_DEEPSLEEP:
      strcpy_P(buff, PSTR("Deep-Sleep Wake"));
      break;
    case ESP_RST_BROWNOUT:
      strcpy_P(buff, PSTR("Brownout"));
      break;
    case ESP_RST_SDIO:
      strcpy_P(buff, PSTR("SDIO"));
      break;
    default:
      strcpy_P(buff, PSTR("Unknown"));
      break;
  }

  return String(buff);
}

/**
 * 
*/
String DTOsServices::fromHubToStatusJsonString(Hub* hub){
  String status;
  
  DynamicJsonBuffer jBuffer;
  JsonObject& root = jBuffer.createObject();
  JsonObject& esp = root.createNestedObject("esp");

  
  // Firmware version.
  root["firmware"]  = hub->getFirmwareVersion();  
  // Sound last data read
  root["lastSoundLevel"] = hub->soundLastValue();  

  esp_reset_reason_t reason = esp_reset_reason();

  esp["reset_reason"] = getResetReason(reason);
  esp["chip_id"] = hub->getMonitorIdString();
  //esp["core_version"] = ESP.getCoreVersion();
  esp["skd_version"] = ESP.getSdkVersion();
  esp["sketch_size"] = ESP.getSketchSize();
  esp["free_sketch_space"] = ESP.getFreeSketchSpace();
  esp["sketch_md5"] = ESP.getSketchMD5();
  //esp["flash_chip_id"] = ESP.getFlashChipId();
  esp["flash_chip_size"] = ESP.getFlashChipSize();
  //esp["flash_chip_real_size"] = ESP.getFlashChipRealSize(); 
  
  root.printTo(status);
  return status;
}


/**
 * 
*/
String DTOsServices::firmStatusJsonString(String msgFirm){
  String msgToSend;
  DynamicJsonBuffer jBuffer;
  JsonObject& root = jBuffer.createObject();
    
  // Firmware version.
  root["firmStatus"]  = msgFirm;
  root.printTo(msgToSend);
  return msgToSend;
}