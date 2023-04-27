#include "WifiConns.h"

/**
 * 
*/
WifiConns::WifiConns(){
}


/**
 * 
*/
boolean WifiConns::someWifiNotEmpty(){  
  boolean wifisSavedInFile = false;

  for (int i = 0; i < WIFI_MAX_CONNECTIONS_SAVED; i++) {
    if(wifis[i].ssid != ""){
      wifisSavedInFile = true;
      break;
    }
  }
  // 
  return wifisSavedInFile;
}

/**
 * Add a new connection to array of connections. 
 * in lastIndex position. 
 */
void WifiConns::addNewConn(String ssid, String pass){
  if(lastAddedIndex >= WIFI_MAX_CONNECTIONS_SAVED){
    lastAddedIndex = 0;
  }
  wifis[lastAddedIndex].ssid = ssid;
  wifis[lastAddedIndex].pass = pass;
  lastAddedIndex++;
}

/*
* Add a connection which it is posiblle exist or not.
* 0 no changes (Exists in file and pass not change)
* 1 exists in file, but change password
* 2 Not exists in file. Add to queue.
*/ 

int WifiConns::addConn(String ssid, String pass){  
  
  int index = existSSID(ssid);
  
  // Update pass.
  if(index >= 0){
    if(wifis[index].pass  == pass){
      return CONN_NOT_CHANGE;
    }
    wifis[index].pass = pass;
    return CONN_PASS_CHANGED;
  }  
  // Connection still not in array
  // Add as queue.
  int position = WIFI_MAX_CONNECTIONS_SAVED - 1;
  while(position>0){
    wifis[position] = wifis[position-1];
    position--;
  }
  // First place new connection
  wifis[0].ssid = ssid;
  wifis[0].pass = pass;
  return CONN_NEW;
}

/*
*
*/
int WifiConns::existSSID(String ssid){
  int index = -1;
  for (int i = 0; i < WIFI_MAX_CONNECTIONS_SAVED; i++) {
    if(wifis[i].ssid == ssid){
      index = i;
      break;
    }    
  }
  return index;
}
    



