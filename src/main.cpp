#include <Arduino.h>
#include "Leds.h"
#include "ButtonResetApi.h"
#include "Microphone.h"
#include "MicrophonePTP.h"
#include <Adafruit_Si7021.h>
#include "TemperatureSi7021.h"
#include "HumiditySi7021.h"
#include "Sensors.h"
#include "Hub.h"
#include "AppServices.h"
#include "Battery.h"
#include "WifiConfigAPAsync.h"

//WifiConnectionStats wifiConnectionStats;
// Usar el anterior struct para sustituir esto:
unsigned long currentWifiConnection = 0;
unsigned long lastWifiConnection = 0;
int wifiTimesLost = 0;

/// DEFINE ALL GLOBAL VARIABLES NEEDED
unsigned int disconnected = 0;
volatile bool haveToStartConfigMode = false;
unsigned long lastBtnFlashPressMillis;
int numMQTTErrors = 0;
bool isCharging = true;


Hub* hub = Hub::getInstance();
MqttApi mqttApi = MqttApi(hub->getMonitorIdString());
WiFiClient espClient;
Leds leds;
ButtonResetApi button(13);
Microphone microApi(35);
//MicrophonePTP microApi(35);
Adafruit_Si7021 si7021_sensor = Adafruit_Si7021();
HumiditySi7021 humidity = HumiditySi7021(&si7021_sensor);
TemperatureSi7021 temperature = TemperatureSi7021(&si7021_sensor);
Battery battery(14,34);
Sensors sensors = Sensors(&battery,&microApi,&humidity,&temperature);

WifiConns wifiConns = WifiConns();
TimerHub timers = TimerHub();

WifiConfigurator * wificonf =  new WifiConfigAPAsync("Roomonitor","12345678","roomonitor");


///////////////////////////////////////////////////////////////////////////////
/**
 * 
 */ 
boolean hasMQQTConnection(){

  // Sometimes when server keep alive is reached, server close connection but clientMqtt.connected 
  // is true and all publish methods fails. 
  // This a hack to disconnect from MQTT server, and then force to reconnect with server 
  if(mqttApi.tooManyPublishError()){
    DEBUG_PRINT("Disconecting from MQTT. Too many publish message error. Reconnecting...");
    mqttApi.disconnectAndResetValues();    
  }

  if (mqttApi.clientMQTT.connected()) {
    leds.blinkOff();
    return true;
  }
  // Not connected. Retry connection and subscriptions  
  DEBUG_PRINT("Not connected to MQTT server");
  leds.blinkBlue(0.3);
  DEBUG_PRINT("Attempting to connect to MQTT server");        
  if(mqttApi.reconnectToServer()){	
    DEBUG_PRINT("MQTT server connected and Client subscribes all topic");     
    leds.blinkOff();
    return true;
  }
  // Not connected to MQTT Server...  
  DEBUG_PRINT("No connected to MQTT, or error when subscribe all topics. Delay 5000 and init loop"); 
  DEBUG_PRINT("MQTT State:" + String(mqttApi.clientMQTT.state()));  
  numMQTTErrors +=1;
  // 
  if(numMQTTErrors >= 5){
    hub->resetSettings();
    numMQTTErrors = 0;
  }  
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * 
 */
void checkNewMqqtMessage(char* topic, byte* payload, unsigned int length){

  String topicAsString = topic;
  String payloadAsString = "";
  
  // Comparte buffer con el publish. Asi que es mejor copiar el mensaje 
  // exacto en un String nuevo.
  for (int i=0;i<length;i++) {
    payloadAsString += (char)payload[i];    
  }
    
  // Topic HubStatusRequest: Request to send Hub status via MQTT.  
  if (mqttApi.isTopicHubStatusRequest(topicAsString)){
    HubServices::requestStatus();
    return;
  }
  // Topic settings: 
  if (mqttApi.isTopicSettings(topicAsString)){    
    HubServices::requestSettings();
    return;
  }
  
  // Topic roomonitor/espId/wifi: Reset WIFI settings:  
  if (mqttApi.isTopicWifiReset(topicAsString)){    
    /*WiFiManager wifiManager;
    wifiManager.resetSettings();*/
    DEBUG_PRINT("Reset WIFI Settings");
    delay(1000);
    haveToStartConfigMode = true;
    return;
  }

  // Topic roomonitor/espId/portal: Start Portal Mode via MQTT
  if (mqttApi.isTopicStartPortal(topicAsString)){
    DEBUG_PRINT("Starting Portal Mode requested from server");
    delay(1000);
    haveToStartConfigMode = true;
    return;
  }
  
  // Topic roomonitor/espId/mic: Change microphone resistence.
  if (mqttApi.isTopicChangeResistance(topicAsString)){      
      if (payloadAsString != "") {
        DEBUG_PRINT("Resistance changed to " + payloadAsString);
        microApi.changeResistance(payloadAsString.toInt());
      }
      return;
  }
  
  // Topic roomonitor/espId/firmware  
  if (mqttApi.isTopicFirmware(topicAsString)){
    DEBUG_PRINT("Updating firmware requested from server");    
    bool resetAfterOTA = false;
    leds.blinkBlue(0.1);
    delay(1000);
    resetAfterOTA = HubServices::updateFirmware(payloadAsString);
    // Download ok, reset to update new firmware
    if(resetAfterOTA){
      delay(3000);
      ESP.restart();
      delay(5000);
    } 
    return;
  }
        
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  DEBUG_PRINT("Connected");
  WifiServices::saveLastWifiChoosen();
  leds.blinkOff();
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  leds.allOff();
  DEBUG_PRINT("Disconnected");
}

void setup()
{
  Serial.begin(115200); 
  
  /*if (wificonf->start())
  {
    Serial.println("Started");
  }
  else
  {
    Serial.println("Error");
  }
  Serial.println(WiFi.softAPgetHostname());
  Serial.println(WiFi.softAPIP());
  return;*/
  WiFi.mode(WIFI_STA);
  leds.setup();
  button.setup();
  microApi.setup();
  battery.setup();
  mqttApi.setup(espClient);
  mqttApi.clientMQTT.setCallback(checkNewMqqtMessage);
  si7021_sensor.begin();
  WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_STOP);
  //DEBUG_PRINT("Loading connections");
  delay(5000);
  WifiServices::loadConnections();
  
  // Manual connection to the network
  //WiFi.begin("MareaXXI", "vuYPE54p");
  //WiFi.begin("INFINITUM0A7B", "0723937786");
  
  timers.setup();

  //DEBUG_PRINT("Setup Testing Blue and Red Led Blinks...");
  leds.startTesting(500); 
  delay(1000);

  // RED & BLUE LED OFF before WiFi connected
  leds.allOff();

  if (!WifiServices::someWifiAvailable())
  {
   // DEBUG_PRINT("We haven't got any SSID and PASS credentials, so get them now");
    haveToStartConfigMode = true;
  }
}

void loop()
{

  button.saveTimeNotPressed();

  if (haveToStartConfigMode || button.isPressedDuringTime(5000))
  {
    //DEBUG_PRINT("Button Reset pressed > 5 seconds or startConfigMode == true");
    delay(1);
    leds.blinkBlue(0.3);
    WifiServices::startModeConfig();

        // Always reset when exit portal mode
    //DEBUG_PRINT("Delay and reset when exit portal mode");
    leds.allOff();
    delay(3000);    
	  ESP.restart();
	  delay(5000); 
  }

  //isCharging = battery.readPowerIn();
  isCharging = 1; //Fixed until solve the hardware issues

  switch (timers.getMode())
  {
  case TimerHub::Mode::NORMAL:
    if (!isCharging)
    {
      DEBUG_PRINT("Changing to ECO mode");
      timers.changeToEcoMode();
      if (!WiFi.disconnect(true))
      {
        DEBUG_PRINT("Error to disconnect");
      }
    }
    if (!WiFi.isConnected())
    {
      WifiServices::connectWifi(button, leds);
    }
    else
    {
      if(!hub->isSettingsRequested())
      {    
        // If not response settings from server then sleep.
        if(!HubServices::requestSettings())
        {
          DEBUG_PRINT("Not settings requested. Delay 10 seconds");
          delay(10000);
          return;
        }
      }
      //if there is not MQTT connection, then notify via API, sleep and return.
      if(!hasMQQTConnection()){
        HubServices::sendMqttConnectionError();
        DEBUG_PRINT("Not MQTT Connection. Delay 5 seconds");    
        delay(5000);
        return;
      }
    }
    break;
  case TimerHub::Mode::ECO:
    if(isCharging)
    {
      DEBUG_PRINT("Changing to Normal mode");
      timers.changeToNormalMode();
    }
  default:
    break;
  }

  // Connected to WIFI and MQTT Server
  currentWifiConnection = millis();
  
   
  // Check for new micro data: read sound value 
  //SoundServices::loopActionSound(timerMicro, currentWifiConnection ,lastWifiConnection, wifiTimesLost);
  // Ticker set a true this variable, in the next loop cycle execute action and reset variable
  if (timers.isTimeUpdateValues())
  {   
   // DEBUG_PRINT("Updating sound values");
   //int sound_value = sensors.microphone->readSignal();
    int32_t sound_value = sensors.microphone->readSignal();
    hub->soundNewValue(sound_value);
    timers.valuesUpdated();
    //DEBUG_PRINT("Reading sound value: " + String(sound_value));
  }
  if (timers.isTimeUpdateAverage())
  {
    //DEBUG_PRINT("Updating average");
    int average = hub->updateAverage();
    timers.averageUpdated();
    DEBUG_PRINT("Sound average: " + String(average));
    if(!WiFi.isConnected())
    {
      WifiServices::connectWifi(button, leds);
    }
  }
  if (timers.isTimeSendValues())
  {
   // DEBUG_PRINT("Sending sound value");
    if(WiFi.isConnected())
    {
      if(!hub->isSettingsRequested())
      {    
        // If not response settings from server then sleep.
        if(!HubServices::requestSettings())
        {
          DEBUG_PRINT("Not settings requested. Delay 10 seconds");
          delay(10000);
          return;
        }
      }

      //if there is not MQTT connection, then notify via API, sleep and return.
      if(!hasMQQTConnection())
      {
        HubServices::sendMqttConnectionError();
        DEBUG_PRINT("Not MQTT Connection. Delay 5 seconds");    
        delay(5000);
        return;
      }

      SensorsServices::sendSensorsData(currentWifiConnection ,lastWifiConnection, wifiTimesLost, &sensors);
      timers.valuesSent();
    }  
  }

  if(timers.isTimeSendAverage())
  {
   // DEBUG_PRINT("Sending average");
    if(WiFi.isConnected())
    {
      if(!hub->isSettingsRequested())
      {    
        // If not response settings from server then sleep.
        if(!HubServices::requestSettings())
        {
          DEBUG_PRINT("Not settings requested. Delay 10 seconds");
          delay(10000);
          return;
        }
      }

      //if there is not MQTT connection, then notify via API, sleep and return.
      if(!hasMQQTConnection())
      {
        HubServices::sendMqttConnectionError();
        DEBUG_PRINT("Not MQTT Connection. Delay 5 seconds");    
        delay(5000);
        return;
      }

      if (hub->isAverageAvailable())
      {
        if (timers.getMode() == TimerHub::Mode::NORMAL)
        {
          DEBUG_PRINT("Single average");
          SensorsServices::sendSensorsSingleAverage(currentWifiConnection, lastWifiConnection, wifiTimesLost, &sensors);
        }
        else
        {
          DEBUG_PRINT("Array average");
          SensorsServices::sendSensorsArrayAverage(currentWifiConnection, lastWifiConnection, wifiTimesLost, &sensors);
          if (!WiFi.disconnect(true))
          {
            DEBUG_PRINT("Error to disconnect");
          }
        }
        timers.averageSent();
      }
      else
      {
        DEBUG_PRINT("Not average available");
      }
      
    }
  }

  // Reset last wifiConnection
  if (lastWifiConnection > 0){
    lastWifiConnection = 0;
  }     

  // Loop mqtt to manage subscriptions topics.
  mqttApi.clientMQTT.loop();
  yield();
}