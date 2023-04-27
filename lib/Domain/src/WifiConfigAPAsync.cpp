#include "WifiConfigAPAsync.h"

std::deque<NetworkScanItem> WifiConfigAPAsync::wifilist;

std::string WifiConfigurator::sta_ssid;
std::string WifiConfigurator::sta_pass;

bool WifiConfigAPAsync::is_connected;

void WifiConfigAPAsync::wifiScanEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
    if (!wifilist.empty())
    {
        wifilist.clear();
    }
    for (uint8_t it = 0; it < 255; it++)
    {
        NetworkScanItem item;
        item.ssid = WiFi.SSID(it).c_str();
        item.rssi = WiFi.RSSI(it);
        if (item.ssid == "")
        {
            break;
        }
        wifilist.push_back(item);
    }
}

void WifiConfigAPAsync::wifiConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    is_connected = true;
}

void WifiConfigAPAsync::notFound(AsyncWebServerRequest *request) 
{
    request->send(404, "text/plain", "Not found");
}

void WifiConfigAPAsync::getInfo(AsyncWebServerRequest *request)
{
    AsyncJsonResponse *response = new AsyncJsonResponse();
    response->addHeader("roomonitor_info","Roomonitor information");
    JsonObject& root = response->getRoot();
    root["firmware_version"] = FIRMWARE_VERSION;
    response->setLength();
    request->send(response);
}

void WifiConfigAPAsync::listWifi(AsyncWebServerRequest *request)
{
    WiFi.scanNetworks();
    delay(100);
    AsyncJsonResponse *response = new AsyncJsonResponse();
    response->addHeader("roomonitor_list","Roomonitor wifi list");
    JsonObject& root = response->getRoot();

    JsonArray& networks = root.createNestedArray("networks");
    auto num_nets = wifilist.size();
    for (auto w_it = 0; w_it < num_nets; ++w_it)
    {
        //NetworkScanItemFormatter f_item(wifilist.at(w_it));
        JsonObject& obj = networks.createNestedObject();
        //networks.add(obj);
        String rssi_str(wifilist.at(w_it).rssi);
        obj["ssid"] = wifilist.at(w_it).ssid.c_str();
        obj["strength"] = rssi_str;
    }

    response->setLength();
    request->send(response);
}

void WifiConfigAPAsync::connect(AsyncWebServerRequest *request)
{
    AsyncJsonResponse *response = new AsyncJsonResponse();
    response->addHeader("roomonitor_connect","Roomonitor connect to net");
    JsonObject& root = response->getRoot();

    int status = 0;
    String ssid,pass = "";

    if (request->hasParam("s"))
    {
        ssid = request->getParam("s")->value();
    }
    if (request->hasParam("p"))
    {
        pass = request->getParam("p")->value();
    }
    if (ssid == "")
    {
        root["status"] = status;
        root["msg"] = "SSID shouldn't empty";
        response->setLength();
        request->send(response);
        return;
    }
    wl_status_t wl_res = WL_DISCONNECTED;
    if (pass == "")
    {
        wl_res = WiFi.begin(ssid.c_str());

    }
    else
    {
        wl_res = WiFi.begin(ssid.c_str(),pass.c_str());
    }
    
    
    wl_res = (wl_status_t)WiFi.waitForConnectResult();

    if (wl_res != WL_CONNECTED)
    {
        status = 0;
        root["status"] = status;
        root["msg"] = "Error when tried to connect";
        response->setLength();
        request->send(response);
        return;
    }
    sta_pass = pass.c_str();
    sta_ssid = ssid.c_str();
    status = 1;
    root["status"] = status;

    response->setLength();
    request->send(response);
}

WifiConfigAPAsync::WifiConfigAPAsync(string ap_ssid, string ap_pass, string hostname, uint16_t port)
{
    webserver = new AsyncWebServer(port);
    webserver->onNotFound(notFound);

    this->ap_pass = ap_pass;
    this->ap_ssid = ap_ssid;
    this->hostname = hostname;

    this->sta_pass = "";
    this->sta_ssid = "";

    webserver->on("/info", HTTP_GET, getInfo);
    webserver->on("/wifi", HTTP_GET, listWifi);
    webserver->on("/wifisave", HTTP_GET, connect);
    WiFi.onEvent(wifiScanEvent,WiFiEvent_t::SYSTEM_EVENT_SCAN_DONE);
    WiFi.onEvent(wifiConnected,WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
    is_connected = false;
}

bool WifiConfigAPAsync::start()
{
    bool result = true;
    //WiFi.disconnect(true,true)
    //esp_wifi_restore();
    if (!WiFi.isConnected())
    {
        WiFi.persistent(false);
        WiFi.disconnect();
        WiFi.mode(WIFI_AP);
        WiFi.persistent(true);
        /*if (!WiFi.disconnect(true,true))
        {
            return false;
        }*/
    }else
    {
        WiFi.mode(WIFI_AP_STA);
    }

    if (this->ap_pass == "")
    {
        if (!WiFi.softAP(this->ap_ssid.c_str()))
        {
            return false;
        }
    }
    else
    {
        if (!WiFi.softAP(this->ap_ssid.c_str(),this->ap_pass.c_str()))
        {
            return false;
        }
    }

    if (this->hostname != "")
    {
        while ( String("0.0.0.0") == WiFi.softAPIP().toString());
        if (!WiFi.softAPsetHostname(this->hostname.c_str()))
        {
            return false;
        }
    }

    webserver->begin();

    return result;
}

bool WifiConfigAPAsync::end()
{
    delay(1000);
    webserver->end();
    return WiFi.softAPdisconnect();
}

bool WifiConfigAPAsync::isConfigured()
{
    return is_connected;
}

string WifiConfigAPAsync::getSSID()
{
    return this->sta_ssid;
}

string WifiConfigAPAsync::getPass()
{
    return this->sta_pass;
}
