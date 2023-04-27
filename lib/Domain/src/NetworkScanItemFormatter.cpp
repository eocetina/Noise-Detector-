#include "NetworkScanItemFormatter.h"

NetworkScanItemFormatter::NetworkScanItemFormatter(NetworkScanItem& item)
{
    this->item = item;
}

JsonObject& NetworkScanItemFormatter::format()
{
    DynamicJsonBuffer buff;
    JsonObject &obj = buff.createObject();
    obj["ssid"] = item.ssid.c_str();
    obj["strength"] = item.rssi;
    return obj;
}