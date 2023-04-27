#include "HttpApi.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "Hub.h"

using namespace HttpApi; 

/**
 * Request settings configurations from server.   
*/ 
ApiResponse HttpApi::requestSettings(){
  Hub* hub = Hub::getInstance();

  const String settingEndpoint = String(HTTP_API_URL) + "/settings/" + hub->getMonitorIdString();
  ApiResponse response = {HTTP_API_NOT_CONNECTION, ""};
  
  //Check WiFi connection status
  if (WiFi.status() != WL_CONNECTED) { 
    return response;
  }
  // Wifi connected
  HTTPClient http;  //Declare an object of class HTTPClient    
  http.begin(settingEndpoint);  //Specify request destination
  http.addHeader("X-Auth-Token", HTTP_API_AUTH_TOKEN, true, true);
  http.addHeader("X-User-Id", HTTP_API_AUTH_USER, true, true);
  
  //Send the request
  response.httpCode = http.GET();
  
  if (response.httpCode > 0) { //Check the returning code 
    response.payload = http.getString();   //Get the request response payload    
  }
  http.end();   //Close connection
  return response;
 
}

/**
 * Send an error to a API endpoint.
 */
boolean HttpApi::sendError(String errorMessage){

  Hub* hub = Hub::getInstance();
  const String errorEndpoint = String(HTTP_API_URL) + "/errors/" + hub->getMonitorIdString();
  //Check WiFi connection status

  if (WiFi.status() != WL_CONNECTED) { 
    return false;
  }
  // Wifi connected
  boolean response = true;
  HTTPClient http;  //Declare an object of class HTTPClient 
  http.begin(errorEndpoint);  //Specify request destination
  http.addHeader("X-Auth-Token", HTTP_API_AUTH_TOKEN, true, true);
  http.addHeader("X-User-Id", HTTP_API_AUTH_USER, true, true);
  http.addHeader("Content-Type", "application/json"); 
   
  int httpCode = http.POST(errorMessage);
  if (httpCode != HTTP_CODE_OK) { //Check the returning code 
    response = false;
  }
  http.end();   //Close connection
  return response;

}

/**
 * Convet error Code to string.
*/
String HttpApi::errorToString(int codeError){
  String error = "";
  
  if(codeError == HTTP_API_NOT_CONNECTION){
    error = "Imposible to connect with server. WIFI not connected";
  }
  
  if(codeError != HTTP_API_NOT_CONNECTION && codeError < 0){
    error = "Imposible to connect with server. HTTP client error";
  }

  // if(codeError == HTTP_API_CONNECTION_TIMEOUT){
  //   error = "Server TimeOut";
  // } 
  if(codeError > 0 && codeError != HTTP_CODE_OK){
    error = "Server Returned the following Http Status Code:" + String(codeError);
  } 
  
  return error;
}