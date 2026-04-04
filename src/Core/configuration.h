#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Connectivity/ble_configuration.h"

#include <Displays/GC9A01_driver_configuration.h>
#include <Sensors/QMI8658_driver_configuration.h>

namespace Configuration
{
  namespace Display
  {
    static const uint16_t width =  240;
    static const uint16_t height = 240;

    static const GC9A01Driver_Configuration defaultConfiguration =
    {
      .spiHost =           SPI2_HOST,
      .spiFrequencyWrite = 80000000, // 80MHz
      .spiPinClock =       10,
      .spiPinMOSI =        11,
      .spiPinChipSelect =  9,
      .pinReset =          12,
      .pinDataCommand =    8,

      .screenWidth =       width,
      .screenHeight =      height,
    };

    static const uint8_t pinBacklight = 40;
  }

  namespace Accelerometer
  {
    static const QMI8658Driver_Configuration defaultConfiguration =
    {
      .wire =       Wire,
      .i2cAddress = 0x6B,
      .i2cPinSDA =  6,
      .i2cPinSCL =  7,

      .range =   SensorQMI8658::ACC_RANGE_4G,
      .odr =     SensorQMI8658::ACC_ODR_1000Hz,
      .lpfMode = SensorQMI8658::LPF_MODE_0,
    };
  }

  namespace BLE
  {
    static const BLE_Configuration defaultConfiguration =
    {
      .name =                     "Dashboard",
      .serviceUUID =              "4fafc201-1fb5-459e-8fcc-c5c9c331914b",
      .configurationChannelUUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8",
    };
  }
}

#endif // CONFIGURATION_H