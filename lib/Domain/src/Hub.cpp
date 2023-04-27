#include "Hub.h"

Hub* Hub::instance = 0;
unsigned int Hub::eco_mode_interval = 10;
unsigned int Hub::noise_adjustment = 0;
/**
 * 
*/
Hub::Hub(){
  esp_read_mac(monitorId, ESP_MAC_WIFI_STA);
  firmwareVersion = FIRMWARE_VERSION;
  settingRequested = false;
  sound = new std::deque<int>();
  sound_average = new std::queue<int>();
}

/**
 * Singleton pattern. 
*/
Hub* Hub::getInstance(){
  if(!instance) {
			instance = new Hub();			
			return instance;
		}
		else {			
			return instance;
		}
}

void Hub::soundNewValue(int value)
{
  if(sound->size() == MAX_SOUND_SAMPLES)
  {
    sound->pop_front();
  }
  sound->push_back(value - noise_adjustment);
}


int Hub::soundAverage()
{
  int average = sound_average->front();
  sound_average->pop();
  return average;
}

int Hub::updateAverage()
{
  int average =std::accumulate(sound->begin(),sound->end(),0)/sound->size();
  //(std::accumulate(sound->begin(),sound->end(),0))/(sound->size());
  sound_average->push(average);
  if (average > 1024) average=666;
  return 0;
}

bool Hub::isAverageAvailable()
{
  return sound_average->size() > 0;
}

int Hub::soundLastValue()
{
  return sound->back();
}

/**
 * 
*/
String Hub::getFirmwareVersion(){  
  return firmwareVersion;
}

/**
 * 
*/
uint8_t* Hub::getMonitorId(){    
  return monitorId;
}  

/**
 * 
 */

String Hub::getMonitorIdString()
{
  char macStr[13];
  sprintf(macStr, "%02X%02X%02X%02X%02X%02X", monitorId[0], monitorId[1], 
                monitorId[2], monitorId[3], monitorId[4], monitorId[5]);
  return String(macStr);
}

/**
 * 
*/
String Hub::getSSIDAPName(){
  char macStr[34];
  sprintf(macStr, "Roomonitor-%02X%02X%02X%02X%02X%02X", monitorId[0], monitorId[1], 
                monitorId[2], monitorId[3], monitorId[4], monitorId[5]);
  return String(macStr);
}

/** 
 * Reset values hvac saved
 */
void Hub::resetSettings(){  
  settingRequested = false;
}

/** 
 * Reset values hvac saved
 */
boolean Hub::isSettingsRequested(){  
  return settingRequested;  
}
void Hub::settingsWasRequested(){
  settingRequested = true;
}

unsigned int Hub::getECOInterval()
{
  return eco_mode_interval;
}

void Hub::setECOInterval(unsigned int interval)
{
  eco_mode_interval = interval;
}

unsigned int Hub::getNoiseAdjustment()
{
  return noise_adjustment;
}

void Hub::setNoiseAdjustment(unsigned int neg_offset)
{
  noise_adjustment = neg_offset;
}

void Hub::clearSoundAverageQueue()
{
  while(!sound_average->empty())
  {
    sound_average->pop();
  }
}