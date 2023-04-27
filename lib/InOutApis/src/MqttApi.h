#ifndef MqttApi_h
#define MqttApi_h

#define NUM_MAX_PUBLISH_ERR 5     // 5sec

#include <Arduino.h> //Import String class
#include <PubSubClient.h>
#include <WiFi.h>


struct MqttServer{
  String host;  
  int port;
  String user;
  String pass;
};

//#define MQTT_MAX_PACKET_SIZE 512
class MqttApi {
private:
  String hubId;
  
  int numErrorPublish;
   // Topic To Publish
  String topicHubStatus;
  String topicNewSensor;
  String topicNewSound;
  String topicNewHvacData;
  String topicFirmStatus;
  String topicError;

  
  void createSubscribeTopics();
  void createPublishTopics();
  void addPublishError(boolean error);
  
  
public:
  // mqtt data connection      
  PubSubClient clientMQTT = PubSubClient();
  MqttServer mqttServer;
  //
  MqttApi(String espId);
  void setup(WiFiClient& wifiClient);
  void setServer(MqttServer server);
  boolean reconnectToServer();
  void disconnectAndResetValues();
  void subscribeAllTopic();
    //
  boolean publishHubStatus(String status);
  boolean publishNewSensorCode(String msg);   
  boolean publishNewSoundData(String data);   
  boolean publishFirmResult(String msg);
  boolean publishHvacData(String mac, String data);
  boolean publishHvacId(String mac, String data);
  boolean publishError(String error);

   // Topic to Subscribe
  String topicAcStatus;
  String topicAcMode;
  String topicAcLimits;
  String topicAcTemp;
  String topicAcId;
  String topicChangeMicResistence;
  String topicWiFiReset;
  String topicSettings;
  String topicStartPortal;
  String topicFirmware;  
  String topicHubStatusRequest; 

  // Functions in order to know if a topic is ...
  boolean isTopicAc(String topic);
  boolean isTopicAcStatus(String topic);  
  boolean isTopicAcMode(String topic);  
  boolean isTopicAcLimits(String topic);  
  boolean isTopicAcTemp(String topic);  
  boolean isTopicAcId(String topic);
  String searchMacFromTopic(String topic);
  //
  boolean isTopicChangeResistance(String topic);    
  boolean isTopicWifiReset(String topic);  
  boolean isTopicSettings(String topic);  
  boolean isTopicStartPortal(String topic);  
  boolean isTopicFirmware(String topic);
  boolean isTopicHubStatusRequest(String topic);

  boolean tooManyPublishError();
  
};

#endif