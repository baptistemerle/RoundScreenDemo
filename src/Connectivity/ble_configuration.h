#ifndef BLE_CONFIGURATION_H
#define BLE_CONFIGURATION_H

#include <string>

struct BLE_Configuration
{
  const std::string name;
  const std::string serviceUUID;
  const std::string configurationChannelUUID;
};

#endif // BLE_CONFIGURATION_H