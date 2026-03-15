#include "main_app.h"

#include "configuration.h"

MainApp::MainApp()
  : m_displayDriver(Configuration::Display::defaultConfiguration)
  , m_qmi8658(Configuration::Accelerometer::defaultConfiguration)
  , m_displayEngine(m_displayDriver)
  , m_bleController(Configuration::BLE::defaultConfiguration)
  , m_connectivityController(m_bleController)
  , m_configurationController(m_dashBoardState)
  , m_accelerometerController(m_qmi8658, m_dashBoardState)
{}

void MainApp::setup()
{
  Serial.begin(115200);

  m_bleController.setConfigurationChunkCallback([this](const std::string& data){ m_configurationController.pushNewData(data); });
  m_bleController.init();
  m_bleController.startAdvertising();

  m_dashBoardState.limit.attach([this](const int& newLimit)
  {
    Serial.println("Update dashboard with limit");
    m_dashBoard.updateLabel("Limit : " + std::to_string(newLimit));
  });

  m_dashBoardState.arcValue.attach([this](const int& arcValue)
  {
    m_dashBoard.updateArc(constrain(arcValue, 0, 100));
  });

  // Wait serial port for max 4s if needed
  uint32_t startTime = millis();
  while (!Serial && (millis() - startTime < 4000));

  Serial.println("\n=== DEBUG CONSOLE ACTIVE ===");

  bool accelerometerOK = m_qmi8658.init();

  lv_init();

  lv_disp_t* display = m_displayEngine.init();
  m_dashBoard.init(display);

  pinMode(Configuration::Display::pinBacklight, OUTPUT);
  digitalWrite(Configuration::Display::pinBacklight, HIGH);

  const esp_timer_create_args_t tick_timer_args =
  {
    .callback = &MainApp::lv_tick_task,
    .arg = this,
    .dispatch_method = ESP_TIMER_TASK,
    .name = "lv_tick",
    .skip_unhandled_events = false
  };
  esp_timer_handle_t lv_tick_timer;
  esp_timer_create(&tick_timer_args, &lv_tick_timer);
  esp_timer_start_periodic(lv_tick_timer, 1000);

  if (accelerometerOK)
  {
    m_dashBoard.updateLabel("version 2");
  }
  else
  {
    m_dashBoard.updateLabel("QMI init failed");
  }
}

void MainApp::loop()
{
  lv_timer_handler();

  m_bleController.process();

  m_accelerometerController.update();

  delay(5);
}

void MainApp::lv_tick_task(void *arg)
{
  lv_tick_inc(1);
}