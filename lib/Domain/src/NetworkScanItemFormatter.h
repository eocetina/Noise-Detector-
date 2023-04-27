#ifndef NETWORKSCANITEMFORMATTER_H
#define NETWORKSCANITEMFORMATTER_H

#include "ArdJsonFormatter.h"
#include "NetworkScanItem.h"

class NetworkScanItemFormatter: public ArdJsonFormatter
{
private:
    NetworkScanItem item;
public:
    NetworkScanItemFormatter(NetworkScanItem& item);
    JsonObject &format();
};


#endif