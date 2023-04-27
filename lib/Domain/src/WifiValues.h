#ifndef WifiValues_h
#define WifiValues_h

#include <Arduino.h> //Import String class
#include <WiFi.h>

class WifiValues{  
  public:      
    //
    WifiValues();
    static String quality();
    static String localIp();
    static String timeOut(unsigned long currentWifiConnection, unsigned long lastWifiConnection);
    
};


#endif