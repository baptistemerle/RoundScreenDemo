#include <Arduino.h>

#include "ble_controller.h"
#include "connectivity_controller.h"
#include "configuration.h"
#include "configuration_controller.h"
#include "dashboard_state.h"
#include "dashboard_ui.h"
#include "display_engine.h"
#include "GC9A01_driver.h"
#include "QMI8658_driver.h"

GC9A01Driver displayDriver(Configuration::Display::defaultConfiguration);
QMI8658Driver qmi8658(Configuration::Accelerometer::defaultConfiguration);

DisplayEngine displayEngine(displayDriver);
DashBoardUI dashBoard;

BLEController bleController(Configuration::BLE::defaultConfiguration);
ConnectivityController connectivityController(bleController);

DashBoardState dashBoardState;
ConfigurationController configurationController(dashBoardState);

static void lv_tick_task(void *arg)
{
  lv_tick_inc(1);
}

void setup()
{
  Serial.begin(115200);

  bleController.setConfigurationChunkCallback([=](const std::string& data){ configurationController.pushNewData(data); });
  bleController.init();
  bleController.startAdvertising();

  dashBoardState.limit.attach([](const int& newLimit)
  {
    Serial.println("Update dashboard with limit");
    dashBoard.updateLabel("Limit : " + std::to_string(newLimit));
  });

  // Wait serial port for max 4s if needed
  uint32_t startTime = millis();
  while (!Serial && (millis() - startTime < 4000));

  Serial.println("\n=== DEBUG CONSOLE ACTIVE ===");

  bool accelerometerOK = qmi8658.init();

  lv_init();

  lv_disp_t* display = displayEngine.init();
  dashBoard.init(display);

  pinMode(Configuration::Display::pinBacklight, OUTPUT);
  digitalWrite(Configuration::Display::pinBacklight, HIGH);

  const esp_timer_create_args_t tick_timer_args =
  {
    .callback = &lv_tick_task,
    .name = "lv_tick"
  };
  esp_timer_handle_t lv_tick_timer;
  esp_timer_create(&tick_timer_args, &lv_tick_timer);
  esp_timer_start_periodic(lv_tick_timer, 1000);

  if (accelerometerOK)
  {
    dashBoard.updateLabel("version 1");
  }
  else
  {
    dashBoard.updateLabel("QMI init failed");
  }
}

void loop()
{
  lv_timer_handler();

  bleController.process();

  float accX, accY, accZ;

  if (qmi8658.getValues(accX, accY, accZ))
  {
    int arcVal = map(accX * 100, -100, 100, 0, 100);
    dashBoard.updateArc(constrain(arcVal, 0, 100));
    Serial.printf("X: %.2f  Y: %.2f  Z: %.2f\n", accX, accY, accZ);
  }

  delay(5);
}
