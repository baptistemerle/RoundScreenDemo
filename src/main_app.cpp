#include "main_app.h"

#include "configuration.h"

MainApp::MainApp()
  : m_displayDriver(Configuration::Display::defaultConfiguration)
  , m_qmi8658(Configuration::Accelerometer::defaultConfiguration)
  , m_displayEngine(m_displayDriver)
  , m_bleController(Configuration::BLE::defaultConfiguration)
  , m_connectivityController(m_bleController, m_dashBoardState)
  , m_jsonConfigurationParser(m_dashBoardState)
  , m_accelerometerController(m_qmi8658, m_dashBoardState)
{}

void MainApp::setup()
{
  Serial.begin(115200);

  m_bleController.setConfigurationChunkCallback([this](const std::string& data){ m_jsonConfigurationParser.pushNewData(data); });
  m_bleController.init();
  m_bleController.startAdvertising();

  m_dashBoardState.limit.attach([this](const int& newLimit)
  {
    Serial.println("Update dashboard with limit");
    m_dashBoard.updateLimitLabel(newLimit);
  });

  m_dashBoardState.accelerometerXValue.attach([this](const float& xValue)
  {
    m_dashBoard.updateXAxisValue(map(xValue * 100, -100, 100, 0, 100));
  });

  m_dashBoardState.accelerometerYValue.attach([this](const float& yValue)
  {
    m_dashBoard.updateYAxisValue(map(yValue * 100, -100, 100, 0, 100));
  });

  m_dashBoardState.accelerometerZValue.attach([this](const float& zValue)
  {
    m_dashBoard.updateZAxisValue(map(zValue * 100, -100, 100, 0, 100));
  });

  m_dashBoardState.bluetootDeviceConnected.attach([this](const bool& deviceConnected)
  {
    m_dashBoard.updateBluetoothLabel(deviceConnected);
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
    m_dashBoard.updateVersionLabel("v3");
  }
  else
  {
    m_dashBoard.updateVersionLabel("QMI Failed");
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