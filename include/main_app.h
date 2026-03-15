#ifndef MAIN_APP_H
#define MAIN_APP_H

#include "accelerometer_controller.h"
#include "ble_controller.h"
#include "connectivity_controller.h"
#include "configuration_controller.h"
#include "dashboard_state.h"
#include "dashboard_ui.h"
#include "display_engine.h"
#include "GC9A01_driver.h"
#include "QMI8658_driver.h"

class MainApp
{
public:
  MainApp();

  void setup();
  void loop();

private:
  static void lv_tick_task(void *arg);

private:
  GC9A01Driver m_displayDriver;
  QMI8658Driver m_qmi8658;

  DisplayEngine m_displayEngine;
  DashBoardUI m_dashBoard;

  DashBoardState m_dashBoardState;

  BLEController m_bleController;
  ConnectivityController m_connectivityController;

  ConfigurationController m_configurationController;
  AccelerometerController m_accelerometerController;
};

#endif // MAIN_APP_H