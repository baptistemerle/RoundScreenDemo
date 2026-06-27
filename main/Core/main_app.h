#ifndef MAIN_APP_H
#define MAIN_APP_H

#include "Connectivity/ble_controller.h"
#include "Connectivity/connectivity_controller.h"
#include "Core/dashboard_state.h"
#include "Decoders/json_configuration_parser.h"
#include "Sensors/accelerometer_controller.h"
#include "Storage/preferences_controller.h"
#include "UI/dashboard_ui.h"

#include <Displays/GC9A01_driver.h>
#include <Graphics/display_engine.h>
#include <Sensors/QMI8658_driver.h>

class MainApp
{
public:
  MainApp();

  void setup();
  void loop();

private:
  void initHardware();
  void connectAll();

  static void lv_tick_task(void* arg);

private:
  i2c_master_bus_handle_t m_i2c0_busHandle;

  GC9A01Driver m_displayDriver;
  QMI8658Driver m_qmi8658;

  DisplayEngine m_displayEngine;
  DashBoardUI m_dashBoard;

  DashBoardState m_dashBoardState;

  BLEController m_bleController;
  ConnectivityController m_connectivityController;

  JsonConfigurationParser m_jsonConfigurationParser;
  AccelerometerController m_accelerometerController;
  PreferencesController m_preferencesController;
};

#endif // MAIN_APP_H