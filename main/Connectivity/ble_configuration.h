#ifndef BLE_CONFIGURATION_H
#define BLE_CONFIGURATION_H

#include <string_view>

struct BLE_Configuration
{
  std::string_view name;
  std::string_view serviceUUID;
  std::string_view configurationChannelUUID;
};

#endif // BLE_CONFIGURATION_H