#ifndef WifiServices_h
#define WifiServices_h

#include <Arduino.h> //Import String class
#include <WifiConnsFileRepo.h>
#include <ButtonResetApi.h>
#include <Leds.h>
#include "Hub.h"
#include "WifiConfigAPAsync.h"


namespace WifiServices{
  
  void loadConnections();  
  void saveLastWifiChoosen();
  
  boolean someWifiAvailable();

  void debugConnections();
  void debugWaitForConnectionResult(int resutl);
  
  int connectWifi(ButtonResetApi btnReset, Leds leds);
  //uint8_t waitForConnectResult();

  boolean startModeConfig();
  

}

#endif
