#include "HubServices.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WifiValues.h>
#include <Hub.h>
#include <Arduino.h>
#include <InOutApis.h>
#include <HTTPUpdate.h>
#include <DTOsServices.h>


using namespace HubServices;

extern MqttApi mqttApi;
///////////////////////////////////////////////////////////////////////////////
/**
 * Request setting to server. 
 * Read hvac settings for this hub, and mqtt server connection configuration.
 */
bool HubServices::requestSettings(){
  DEBUG_PRINT("Requesting Settings To URL:" + String(HTTP_API_URL));
  
  Hub* hub = Hub::getInstance();  
  hub->resetSettings(); 
  HttpApi::ApiResponse response = HttpApi::requestSettings();  
  
  if(response.httpCode != HTTP_CODE_OK){
    DEBUG_PRINT("Impossible to request Setting endpoint");
    DEBUG_PRINT(HttpApi::errorToString(response.httpCode));    
    return false;
  }
  
  // Save settings  
  DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(response.payload);
  
  if (!root.success()) {
    DEBUG_PRINT("Error Parsing Settings Response to JSON"); 
    return false;
  }
  
  saveEcoSettings(root);
  saveMqttSettings(root);
  saveMicSettings(root);
  
  // Mark hub has successfully requested setting 
  hub->settingsWasRequested();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
/**
 *
 */
void HubServices::saveMqttSettings(JsonObject& root){    
  DEBUG_PRINT("Saving MQTT Settings"); 
  MqttServer connection;
  // Check if there is not values, set default values.  
  connection.host = root["mqtt"]["host"].as<String>();
  connection.port = (int)root["mqtt"]["port"];
  connection.user = root["mqtt"]["user"].as<String>();
  connection.pass = root["mqtt"]["pass"].as<String>();  
  mqttApi.setServer(connection);

}

/**
 * @brief Save the interval time to send average in ECO mode
 * 
 * @param root Received JSON object
 */

void HubServices::saveEcoSettings(JsonObject& root){    
  
  if (root.containsKey("eco_interval"))
  {
    Hub::setECOInterval( (uint32_t)((int)root["eco_interval"]));
  }
}

/**
 * @brief Save the offset value to substract from mic output
 * 
 * @param root Received JSON object
 */

void HubServices::saveMicSettings(JsonObject& root){    
  
  if (root.containsKey("noise_ajustment"))
  {
    Hub::setNoiseAdjustment( (uint32_t)((int)root["noise_ajustment"]));
    DEBUG_PRINT("noise adjustment");
    DEBUG_PRINT ((int)root["noise_ajustment"]);
  }
  else
  {
    DEBUG_PRINT("No noise adjustment");
  }
}

///////////////////////////////////////////////////////////////////////////////
/**
 * Return hub status, in json string
 **/
void HubServices::requestStatus(){  
  DEBUG_PRINT("Requesting HUB Status");
  Hub* hub = Hub::getInstance();  
  boolean publishOk = false;
  String status = DTOsServices::fromHubToStatusJsonString(hub);  
  // Send status via mqttOutput.
  publishOk = mqttApi.publishHubStatus(status);
  if(!publishOk){
   DEBUG_PRINT("Imposible to publish hub status message");   
  }
}
///////////////////////////////////////////////////////////////////////////////
/**
 * 
 */
void HubServices::sendMqttConnectionError(){
  DEBUG_PRINT("Sending MQTT connection error");
  String errorJson= DTOsServices::fromMqttConnectionErrorToJsonString(mqttApi.mqttServer);  
  boolean response;  
  response = HttpApi::sendError(errorJson);
  if(response == false){
    DEBUG_PRINT("Imposible to send MQTT Connection Error via API");    
  }  
}

///////////////////////////////////////////////////////////////////////////////
/**
 *
 */
bool HubServices::updateFirmware(String firmwareUrl){  
  String msgFirm;
  
  bool hasToUpdate = false;  
  DEBUG_PRINT("Update firmware requesting by server");  

  if (WiFi.status() != WL_CONNECTED) {  
    DEBUG_PRINT("Imposible to update remote firmware. Not Wifi connection");
    return false;
  }

  if(firmwareUrl == ""){
    firmwareUrl = String(OTA_BINARY_DEFAULT);
  }
  DEBUG_PRINT(firmwareUrl);
  httpUpdate.rebootOnUpdate(false);  
  WiFiClient wclient;
  t_httpUpdate_return ret = httpUpdate.update(wclient,firmwareUrl);
    
  switch(ret) {
      case HTTP_UPDATE_FAILED:        
        DEBUG_PRINT(httpUpdate.getLastErrorString());
        msgFirm =  httpUpdate.getLastErrorString();
        break;

      case HTTP_UPDATE_NO_UPDATES:
        DEBUG_PRINT("HTTP_UPDATE_NO_UPDATES");        
        msgFirm = "HTTP_UPDATE_NO_UPDATES";
        break;

      case HTTP_UPDATE_OK:
        hasToUpdate = true;
        DEBUG_PRINT("HTTP_UPDATE_OK");        
        msgFirm = "HTTP_UPDATE_OK";
        break;

      default:
        DEBUG_PRINT("Undefined HTTP_UPDATE Code:");                
        msgFirm = "Undefined HTTP_UPDATE Code:" + String(ret);

  }

  // Write json message and send via MQTT.
  String msgToSend = DTOsServices::firmStatusJsonString(msgFirm);
  mqttApi.publishFirmResult(msgToSend);
  return hasToUpdate;
}
