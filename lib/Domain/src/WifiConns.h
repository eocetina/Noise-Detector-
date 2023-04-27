#ifndef WifiConns_h
#define WifiConns_h

#include <Arduino.h> //Import String class
#include <array>

#define WIFI_MAX_CONNECTIONS_SAVED 5

#define CONN_NOT_CHANGE  -1
#define CONN_PASS_CHANGED  1
#define CONN_NOT_EXIST 2
#define CONN_NEW 3


struct WifiData{
  String ssid;  
  String pass;  
};

typedef std::array<WifiData, WIFI_MAX_CONNECTIONS_SAVED> ArrayWifiData;

class WifiConns{
  private:            
    uint8_t lastAddedIndex = 0;
  public:    
    ArrayWifiData wifis;

    //
    WifiConns();   
        
    boolean someWifiNotEmpty();
    void addNewConn(String ssid, String pass); 
    int addConn(String ssid, String pass); 
    int existSSID(String ssid);       
    
};

#endif