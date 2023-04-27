#include "MqttApi.h"

//: clientMQTT(client)
MqttApi :: MqttApi(String espId ) {
  hubId = espId;    
  mqttServer.port = 1883;
  mqttServer.host = "";
  mqttServer.user = "";
  mqttServer.pass = "";
  numErrorPublish = 0;
}

/**
 * 
 */
void MqttApi::setup(WiFiClient& wifiClient) {  
  clientMQTT.setClient(wifiClient);
  
  // Create All topic to subscribe:
  createSubscribeTopics();  
  // Create All topic to publish:
  createPublishTopics(); 
  
}

/**
 * 
 */
void MqttApi::setServer(MqttServer server){
  boolean serverHasChanged = false;
  if(mqttServer.host != server.host){
    serverHasChanged=true;
  }
  if(mqttServer.port != server.port){
    serverHasChanged=true;
  }
  if(mqttServer.user != server.user){
    serverHasChanged=true;
  }
  if(mqttServer.pass != server.pass){
    serverHasChanged=true;
  }


  mqttServer.host = server.host;
  mqttServer.port = server.port;
  mqttServer.user = server.user;
  mqttServer.pass = server.pass;
  
  clientMQTT.setServer((char*)mqttServer.host.c_str(), mqttServer.port); 
  if(serverHasChanged){
    clientMQTT.disconnect();
  }
}

/**
 * 
 */
void MqttApi::createSubscribeTopics(){
  //String hubIdString = String(hubId);

  topicChangeMicResistence = "roomonitor/" + hubId + "/mic";
  topicWiFiReset = "roomonitor/" + hubId + "/wifi";
  topicSettings = "roomonitor/" + hubId + "/settings";
  topicStartPortal = "roomonitor/" + hubId + "/portal";
  topicFirmware = "roomonitor/" + hubId + "/firmware";  
  topicHubStatusRequest = "roomonitor/" + hubId + "/hub/status/request";
}

/**
 * 
 */
void MqttApi::createPublishTopics(){    
  //String hubIdString = String(hubId);  
  
  topicHubStatus = "roomonitor/" + hubId + "/hub/status";  
  topicNewSound  = "roomonitor/" + hubId + "/sound";
  topicFirmStatus = "roomonitor/" + hubId + "/firmstatus";  
  topicError = "roomonitor/" + hubId + "/error";  
}


/**
 * 
 */
void MqttApi::subscribeAllTopic() {    
  
  clientMQTT.subscribe((char*)topicChangeMicResistence.c_str());
  clientMQTT.subscribe((char*)topicWiFiReset.c_str());
  clientMQTT.subscribe((char*)topicSettings.c_str());
  clientMQTT.subscribe((char*)topicStartPortal.c_str());
  clientMQTT.subscribe((char*)topicFirmware.c_str());  
  clientMQTT.subscribe((char*)topicHubStatusRequest.c_str());  
}

/**
 * 
 */
boolean MqttApi::reconnectToServer(){
  //String hubIdString = String(hubId);
  char * clientId = (char*) hubId.c_str();
  if (clientMQTT.connect(clientId, (char*)mqttServer.user.c_str(), (char*) mqttServer.pass.c_str())){      
    subscribeAllTopic();
    numErrorPublish = 0;
    return true;		  
  }  
  return false;
}

/**
 *
 */
void MqttApi::disconnectAndResetValues(){
  clientMQTT.disconnect();
  numErrorPublish = 0;
}



// ---------------------------------------------------------------------------------------
// ALL MQTT PUBLICATION METHODS
// ---------------------------------------------------------------------------------------

/**
 * In order to save all error when publish
*/
void MqttApi::addPublishError(boolean success){
  if(!success){
    numErrorPublish++;    
  }
}
/**
 *
 */
boolean MqttApi::tooManyPublishError(){
  return numErrorPublish > NUM_MAX_PUBLISH_ERR;
}

/**
 * 
 */
boolean MqttApi::publishHubStatus(String status) {  
  char* payload = (char*) status.c_str();
  boolean isPublished = false;
  isPublished = clientMQTT.publish((char*)topicHubStatus.c_str(), payload);  
  addPublishError(isPublished);
  return isPublished;
  
}

/**
 * 
 */
boolean MqttApi::publishNewSensorCode(String msg){  
  char* payload = (char*) msg.c_str();
  boolean isPublished = false;  
  isPublished = clientMQTT.publish((char*)topicNewSensor.c_str(), payload);
  addPublishError(isPublished);
  return isPublished;

}

/**
 * 
 */
boolean MqttApi::publishNewSoundData(String data){  
  char* payload = (char*) data.c_str();
  boolean isPublished = false;
  isPublished = clientMQTT.publish((char*)topicNewSound.c_str(), payload);
  addPublishError(isPublished);
  return isPublished;
  
}

/**
 * 
 */
boolean MqttApi::publishFirmResult(String msg){
  char* payload = (char*) msg.c_str();
  boolean isPublished = false;  
  isPublished = clientMQTT.publish((char*)topicFirmStatus.c_str(), payload);
  addPublishError(isPublished);
  return isPublished;
}


/**
 * 
 */
boolean MqttApi::publishHvacData(String mac, String data){  
  String topic = topicNewHvacData + mac;   
  char* payload = (char*) data.c_str();
  boolean isPublished = false;
  isPublished = clientMQTT.publish((char*)topic.c_str(), payload);
  addPublishError(isPublished);
  return isPublished;  
}


/**
 * 
 */
boolean MqttApi::publishHvacId(String mac, String data){
  String topic = topicNewHvacData + mac + "/id/data";   
  char* payload = (char*) data.c_str();
  boolean isPublished = false;
  isPublished = clientMQTT.publish((char*)topic.c_str(), payload);
  addPublishError(isPublished);
  return isPublished;  
}

/**
 * 
 */
boolean MqttApi::publishError(String error){  
  char* payload = (char*) error.c_str();
  boolean isPublished = false;  
  isPublished = clientMQTT.publish((char*)topicError.c_str(), payload); 
  addPublishError(isPublished);
  return isPublished;  
}
 
/**
 * 
 */ 
boolean MqttApi::isTopicAc(String topic){  
  String topicAc = "roomonitor/" + hubId + "/ac/";
  return topic.startsWith(topicAc);  
}
/**
 * 
 */
boolean MqttApi::isTopicAcStatus(String topic){      
  int lastIndex = topic.lastIndexOf("/");  
  return topic.substring(lastIndex+1) == "status";
  
}
/**
 * 
 */
boolean MqttApi::isTopicAcMode(String topic){  
  int lastIndex = topic.lastIndexOf("/");  
  return topic.substring(lastIndex+1) == "mode";
}
/**
 * 
 */
boolean MqttApi::isTopicAcLimits(String topic){  
  int lastIndex = topic.lastIndexOf("/");  
  return topic.substring(lastIndex+1) == "limits";
}
/**
 * 
 */
boolean MqttApi::isTopicAcTemp(String topic){  
  int lastIndex = topic.lastIndexOf("/");  
  return topic.substring(lastIndex+1) == "temp";
}

/**
 * 
 */
boolean MqttApi::isTopicAcId(String topic){
  int lastIndex = topic.lastIndexOf("/");  
  return topic.substring(lastIndex+1) == "id";
}
/**
 * 
 */
boolean MqttApi::isTopicChangeResistance(String topic){  
  return topicChangeMicResistence == topic;
}
/**
 * 
 */
boolean MqttApi::isTopicWifiReset(String topic){  
  return topicWiFiReset == topic;
}
/**
 * 
 */
boolean MqttApi::isTopicSettings(String topic){  
  return topicSettings == topic;
}
/**
 * 
 */
boolean MqttApi::isTopicStartPortal(String topic){  
  return topicStartPortal == topic;
}
/**
 * 
 */
boolean MqttApi::isTopicFirmware(String topic){  
  return topicFirmware == topic;
}
/**
 * 
 */
boolean MqttApi::isTopicHubStatusRequest(String topic){  
  return topicHubStatusRequest == topic;
}


/**
 * Given a AC topic string, search the mac substring.
 * */
String MqttApi::searchMacFromTopic(String topic){
  String mac;
  String init = "roomonitor/" + hubId+ "/ac/";
  //Example: roomonitor/123456/ac/AABBCCDD/status
  topic.replace(init, "");
  // Then: AABBCCDD/status  
  int ind1;       
  
  ind1 = topic.indexOf('/');
  mac = topic.substring(0, ind1); 
  return mac;

}