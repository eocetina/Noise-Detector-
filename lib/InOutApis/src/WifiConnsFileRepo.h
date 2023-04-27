#ifndef WifiConnsFileRepo_h
#define WifiConnsFileRepo_h

#include <Arduino.h> //Import String class
#include <WifiConns.h>

//
#define WIFI_CONFIG_NAME "/wificonfig.txt"

namespace WifiConnsFileRepo{
  boolean loadConnections(ArrayWifiData & wifisSaved);
  boolean saveConnections(ArrayWifiData & wifisSaved);
  boolean createNewConfigFile();
}

#endif