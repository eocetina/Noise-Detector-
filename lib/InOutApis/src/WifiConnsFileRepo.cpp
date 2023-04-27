#include "WifiConnsFileRepo.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <DebugUtils.h>


using namespace WifiConnsFileRepo; 

/**
 * Check if the file saved ssid exists.
 * In case of file don't exist, create it.
*/ 
 boolean  WifiConnsFileRepo::loadConnections(ArrayWifiData & wifisSaved){  
  // always use this to "mount" the filesystem
  bool result = SPIFFS.begin(true);
  DEBUG_PRINT("SPIFFS opened: " + String(result));
  if(!result){
    return false;
  }

  if (!SPIFFS.exists(WIFI_CONFIG_NAME))
  {
    DEBUG_PRINT("File not exist. Creating it");
    boolean created = WifiConnsFileRepo::createNewConfigFile(); 
    if(created)
    {
      DEBUG_PRINT("File created");
    }
    return created;
  }
  DEBUG_PRINT("Reading Connections from file");

  File configFile = SPIFFS.open(WIFI_CONFIG_NAME, "r");
  
  if(!configFile){
    DEBUG_PRINT("Failed to open config file.");
    return false;
  }

  // we could open the file
  int totalReaded=0;
  while(configFile.available() && totalReaded < WIFI_MAX_CONNECTIONS_SAVED) {
    //Lets read line by line from the file
    String ssid = "";
    String pass = "";
    ssid = configFile.readStringUntil('\n');
    pass = configFile.readStringUntil('\n');    
    ssid.trim();
    pass.trim();

    wifisSaved[totalReaded].ssid = ssid.substring(5);
    wifisSaved[totalReaded].pass = pass.substring(9);    

    totalReaded++;
    
  }


  configFile.close();
  return true;

}

/**
 * 
*/
boolean WifiConnsFileRepo::saveConnections(ArrayWifiData & wifisSaved){ 

  File configFile = SPIFFS.open("/wificonfig.txt", "w");
  if (!configFile) {
    DEBUG_PRINT("Save wificonfig.txt failed");
    return false;
  }
  for (int i = 0; i < WIFI_MAX_CONNECTIONS_SAVED; i++) {
      String ssid = "ssid=" + wifisSaved[i].ssid;
      String pass = "password=" + wifisSaved[i].pass;
      configFile.println(ssid);
      configFile.println(pass);
  }
  configFile.close();
  return true;
}

/**
 * 
*/
boolean WifiConnsFileRepo::createNewConfigFile(){
// open the file in write mode
  File configFile = SPIFFS.open(WIFI_CONFIG_NAME, "w");
  if (!configFile) {
    DEBUG_PRINT("File creation failed");
    return false;
  }
  DEBUG_PRINT("New config file size: "+String(configFile.position()));
  // now create 5 connections without values.
  configFile.println("ssid=");
  configFile.println("password=");
  
  configFile.println("ssid=");
  configFile.println("password=");
  
  configFile.println("ssid=");
  configFile.println("password=");
  
  configFile.println("ssid=");
  configFile.println("password=");
  
  configFile.println("ssid=");
  configFile.println("password=");
  
  configFile.close();
  DEBUG_PRINT("New config file size: "+String(configFile.position()));
  return true;
}