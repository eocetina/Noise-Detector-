#ifndef HttpApi_h
#define HttpApi_h

#include <Arduino.h> //Import String class

// Define all constant to connect with API Server.
#ifndef HTTP_API_URL
#define HTTP_API_URL "http://admin.roomonitor.com/api"
#endif

#ifndef HTTP_API_PORT
#define HTTP_API_PORT 80
#endif


#ifndef HTTP_API_AUTH_USER
#define HTTP_API_AUTH_USER "4KjZGw7X6W9n5ZGHQ"
#endif

#ifndef HTTP_API_AUTH_TOKEN
#define HTTP_API_AUTH_TOKEN "febDWqimuaL-2u8epYZ9ALDTugGFhTNC_0HU8G74xbm"
#endif

// Possible error values 
#define HTTP_API_NOT_CONNECTION -12
#define HTTP_API_CONNECTION_TIMEOUT -1

namespace HttpApi{
  struct ApiResponse {
    int httpCode;
    String payload;
  };
  ApiResponse requestSettings();
  boolean sendError(String error);
  String errorToString(int codeError);
}

#endif