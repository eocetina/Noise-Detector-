#include "WifiServices.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WifiConns.h>
#include <WifiConnsFileRepo.h>
#include <DebugUtils.h>
//#include <WiFiManager.h>

using namespace WifiServices;

// Declarate in main.
extern WifiConns wifiConns;

/**
 *
 */
void WifiServices::loadConnections(){
  DEBUG_PRINT("Loading wifi conns from file");
  WifiConnsFileRepo::loadConnections(wifiConns.wifis);
  wifiConns.addNewConn("Totalplay-409D", "409DC4A1w7p362Xa");
  //wifiConns.addNewConn("INFINITUM0A7B", "0723937786");
  // wifiConns.addNewConn("MOVISTAR_189D", "rE75zYogU54Euo3gbhTV");
  // wifiConns.addNewConn("GUSANO", "E1agqsapef");
  
  // IN order to save EEPROOM saved last wifi connection to file:
  WifiServices::saveLastWifiChoosen();    
  WifiServices::debugConnections();
}

/**
 * Save the last wifi choosen and save in eeprom
 * If is new connection, save connections to file.
 */
void WifiServices::saveLastWifiChoosen(){
  if(WiFi.SSID() == ""){
    DEBUG_PRINT("NO WIFI SSID");
    return;
  }
  int result = wifiConns.addConn(WiFi.SSID(), WiFi.psk());

  // conn has changed pass or is a new connection which it is not saved in array
  if(result > 0){
    DEBUG_PRINT("Saving wifi connections to file");  
    WifiConnsFileRepo::saveConnections(wifiConns.wifis);
  }

}

/**
 * Search if there is some wifi readed from file repository, or 
 * there is one saved in EEPROOM.
*/
boolean WifiServices::someWifiAvailable(){
                              
  boolean wifiSavedInEEPROM = false;
  boolean wifisSavedInFile = false;

  // Some wifi in eeprom.
  wifiSavedInEEPROM = WiFi.SSID() != "" ;
  // Some wifi saved previously in file
  wifisSavedInFile = wifiConns.someWifiNotEmpty();  

  return wifiSavedInEEPROM || wifisSavedInFile;

} 

/**
 *
 */
int WifiServices::connectWifi(ButtonResetApi btnReset, Leds leds){
  
  if(btnReset.isPressed()){
    return WL_CONNECT_FAILED;
  }    
   
  if (WiFi.status() == WL_CONNECTED) {    
    return WL_CONNECTED;
  }
  leds.blinkBlue(2);  

  // First try to connect to last wifi saved in eeprom, if exists
  if (WiFi.SSID() && WiFi.status() != WL_CONNECTED ) {      
      DEBUG_PRINT("Last WIFI in EEPROOM");
      DEBUG_PRINT(WiFi.SSID());  
      //trying to fix connection in progress hanging
      WiFi.disconnect(false,true);
      //WiFi.begin();
  }
  
  int connRes = WiFi.waitForConnectResult(); 
  
  // Still not connected. Try to connect with saved wifis SSID and pass
  for (int i = 0; i < WIFI_MAX_CONNECTIONS_SAVED && connRes != WL_CONNECTED; i++) {
    
    if(btnReset.isPressed()){
      return WiFi.status() == WL_CONNECTED;
    }    
    if(wifiConns.wifis[i].ssid == ""){
      continue;
    }
    DEBUG_PRINT("Connecting to:");
    DEBUG_PRINT(wifiConns.wifis[i].ssid);    
    WiFi.begin(wifiConns.wifis[i].ssid.c_str(), wifiConns.wifis[i].pass.c_str());
    connRes = WiFi.waitForConnectResult();    
  }

  return WiFi.status();
}

/**
 *
 */
// TODO: Change to bluetooth service
boolean WifiServices::startModeConfig(){
  DEBUG_PRINT("Configuration portal requested.");	
  Hub* hub = Hub::getInstance();
  
  // btnReset.lastBtnPressMillis = millis();   
  //String configPortalSSID = "Roomonitor-" + String(ESP.getChipId());
  delay(10000);
  WifiConfigurator * wificonf =  new WifiConfigAPAsync(hub->getSSIDAPName().c_str(),"");
  
  WiFi.disconnect();
  auto start_time = millis();
  if (!wificonf->start())
  {
    DEBUG_PRINT("Error to init API");
    return false;
  }
  
  while (!wificonf->isConfigured())
  {
    if (millis() - start_time > NUM_SECOND_PORTAL_TIMEOUT*1000)
    {
      DEBUG_PRINT("Timeout to configure to wifi credentials");
      wificonf->end();
      return false;
    }
  }

  // Save current connections
  WifiServices::saveLastWifiChoosen();
  wificonf->end();
  return true;  
}

/**
 *
 */
void WifiServices::debugWaitForConnectionResult(int result){
  String message = "";
  if (result == WL_CONNECTED){
    message = "... connected";
  }
  
  if (result == WL_NO_SSID_AVAIL){
    message = "... SSID cannot be reached";
  }
  
  if (result == WL_CONNECT_FAILED){
    message = "... password is incorrect";
  }

  if (result == WL_IDLE_STATUS){
    message = "... Wi-Fi is in process of changing between statuses ";
  }

  if (result == WL_DISCONNECTED){
    message = "... module is not configured in station mode";
  }  
  DEBUG_PRINT("WaitForConnectionResult:" + message)

}

/**
 *
 */
void WifiServices::debugConnections(){
  for (int i = 0; i < WIFI_MAX_CONNECTIONS_SAVED; i++) {
    DEBUG_PRINT("SSID saved:" + wifiConns.wifis[i].ssid);
    //DEBUG_PRINT("PASS saved:" + wifiConns.wifis[i].pass);
  }
}