#ifndef WIFICONFIGAPASYNC_H
#define WIFICONFIGAPASYNC_H

#include "WifiConfigurator.h"
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <NetworkScanItem.h>
#include <deque>
#include <NetworkScanItemFormatter.h>
#include <Ticker.h>
#include <esp_wifi.h>

class WifiConfigAPAsync : public WifiConfigurator
{
    private:
    AsyncWebServer* webserver;
    string hostname;
    static std::deque<NetworkScanItem> wifilist;
    static bool is_connected;

    static void wifiScanEvent(WiFiEvent_t event, WiFiEventInfo_t info);
    static void wifiConnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void notFound(AsyncWebServerRequest *request);
    static void getInfo(AsyncWebServerRequest *request);
    static void listWifi(AsyncWebServerRequest *request);
    static void connect(AsyncWebServerRequest *request);
    public:
    WifiConfigAPAsync(string ap_ssid, string ap_pass, string hostname = "", uint16_t port = 80);
    bool start();
    bool end();
    bool isConfigured();
    string getSSID();
    string getPass();
};

#endif