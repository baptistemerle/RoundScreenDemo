#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Connectivity/ble_configuration.h"

#include <Displays/GC9A01_driver_configuration.h>
#include <Sensors/QMI8658_driver_configuration.h>

#include <driver/i2c_master.h>
#include <hal/i2c_types.h>

namespace Configuration
{
  namespace Accelerometer
  {
    constexpr QMI8658Driver_Configuration defaultConfiguration =
    {
      .i2cAddress = 0x6B,
      .i2cSpeedHz = 100000,

      .range =   QMI8658Driver_AccelRange::Range_4G,
      .odr =     QMI8658Driver_AccelODR::ODR_250Hz,
      .lpfMode = QMI8658Driver_LPFMode::Mode_2_66,
    };
  }

  namespace BLE
  {
    constexpr BLE_Configuration defaultConfiguration =
    {
      .name =                     "Dashboard",
      .serviceUUID =              "4fafc201-1fb5-459e-8fcc-c5c9c331914b",
      .configurationChannelUUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8",
    };
  }

  namespace Display
  {
    constexpr uint16_t width =  240;
    constexpr uint16_t height = 240;

    constexpr GC9A01Driver_Configuration defaultConfiguration =
    {
      .spiHost =          SPI2_HOST,
      .spiSpeedHz =       80 * 1000 * 1000, // 80MHz
      .spiPinChipSelect = GPIO_NUM_9,
      .pinReset =         GPIO_NUM_12,
      .pinDataCommand =   GPIO_NUM_8,

      .screenWidth =  width,
      .screenHeight = height,
    };

    constexpr gpio_num_t pinBacklight = GPIO_NUM_40;
  }

  namespace I2C
  {
    constexpr i2c_master_bus_config_t i2c0_busConfig =
    {
      .i2c_port =          I2C_NUM_0,
      .sda_io_num =        GPIO_NUM_6,
      .scl_io_num =        GPIO_NUM_7,
      .clk_source =        I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 7,
      .intr_priority =     0,
      .trans_queue_depth = 0,
      .flags = {
        .enable_internal_pullup = true,
        .allow_pd =               false,
      }
    };
  }

  namespace SPI
  {
    constexpr spi_bus_config_t make_spi2_bus_config()
    {
      spi_bus_config_t config = {};

      config.sclk_io_num =     GPIO_NUM_10;
      config.mosi_io_num =     GPIO_NUM_11;
      config.miso_io_num =     -1;
      config.quadwp_io_num =   -1;
      config.quadhd_io_num =   -1;
      config.max_transfer_sz = 64 * 1024;

      return config;
    }

    constexpr spi_bus_config_t spi2_busConfig = make_spi2_bus_config();

    constexpr spi_host_device_t spi2_host = SPI2_HOST;
  }
}

#endif // CONFIGURATION_H