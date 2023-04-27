#include "WifiValues.h"

/**
 * 
*/
WifiValues::WifiValues(){      
}

/**
 * 
*/
String WifiValues::quality(){
  long rssi = WiFi.RSSI();
  if (rssi > -50) {
      rssi = -50;
  }
  int quality = 2 * (rssi + 100);
  return String(quality);
}

/**
 * 
*/
String WifiValues::localIp(){
  String localIP = String(WiFi.localIP()[0]) + "." + String(WiFi.localIP()[1]) + "." + String(WiFi.localIP()[2]) + "." + String(WiFi.localIP()[3]);
  return localIP;
}

/**
 * 
*/
String WifiValues::timeOut(unsigned long currentWifiConnection, unsigned long lastWifiConnection){
  if (lastWifiConnection <= 0) {
    return String(0);
  }
  return String((currentWifiConnection - lastWifiConnection) / 1000);
}
    
