#ifndef WIFICONFIGURATOR_H
#define WIFICONFIGURATOR_H

#include <string>

using std::string;

class WifiConfigurator
{
    protected:
    string ap_ssid;
    string ap_pass;
    static string sta_ssid;
    static string sta_pass;
    public:
    virtual bool start() = 0;
    virtual bool end() = 0;
    virtual bool isConfigured() = 0;
    virtual string getSSID() = 0;
    virtual string getPass() = 0;
};


#endif