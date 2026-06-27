#include "main_app.h"

#include "configuration.h"

#include <driver/gpio.h>
#include <nvs_flash.h>

MainApp::MainApp()
  : m_displayDriver(Configuration::Display::defaultConfiguration)
  , m_qmi8658(Configuration::Accelerometer::defaultConfiguration)
  , m_displayEngine(m_displayDriver)
  , m_bleController(Configuration::BLE::defaultConfiguration)
  , m_connectivityController(m_bleController, m_dashBoardState)
  , m_jsonConfigurationParser(m_dashBoardState)
  , m_accelerometerController(m_qmi8658, m_dashBoardState)
  , m_preferencesController(m_dashBoardState)
{
}

void MainApp::setup()
{
  TickType_t startTick = xTaskGetTickCount();
  const TickType_t timeoutTicks = pdMS_TO_TICKS(4000);

  while ((xTaskGetTickCount() - startTick) < timeoutTicks)
  {
    vTaskDelay(pdMS_TO_TICKS(100));
    break;
  }

  printf("\n=== DEBUG CONSOLE ACTIVE ===\n");

  initHardware();
  connectAll();

  m_preferencesController.init();
  m_preferencesController.loadValues();

  m_bleController.startAdvertising();

  m_dashBoard.updateVersionLabel("v7");
}

void MainApp::loop()
{
  m_bleController.process();

  m_accelerometerController.update();

  uint32_t dynamicDelay = lv_timer_handler();
  if (dynamicDelay < 1)
  {
    dynamicDelay = 1;
  }
  else if (dynamicDelay > 16)
  {
    dynamicDelay = 16;
  }

  vTaskDelay(pdMS_TO_TICKS(dynamicDelay));
}

void MainApp::initHardware()
{
  ESP_ERROR_CHECK(spi_bus_initialize(Configuration::SPI::spi2_host, &Configuration::SPI::spi2_busConfig, SPI_DMA_CH_AUTO));

  ESP_ERROR_CHECK(i2c_new_master_bus(&Configuration::I2C::i2c0_busConfig, &m_i2c0_busHandle));

  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  m_qmi8658.init(m_i2c0_busHandle);

  lv_init();

  lv_disp_t* display = m_displayEngine.init();
  m_dashBoard.init(display);

  gpio_reset_pin(Configuration::Display::pinBacklight);
  gpio_set_direction(Configuration::Display::pinBacklight, GPIO_MODE_OUTPUT);
  gpio_set_level(Configuration::Display::pinBacklight, 1);

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

  m_bleController.init();
}

void MainApp::connectAll()
{
  m_bleController.setConfigurationChunkCallback([this](const std::string& data){ m_jsonConfigurationParser.pushNewData(data); });

  m_dashBoardState.limit.attach([this](const int& newLimit)
  {
    printf("Update dashboard with limit\n");
    m_dashBoard.updateLimitLabel(newLimit);
  });

  m_dashBoardState.accelerometerXValue.attach([this](const float& xValue)
  {
    float mappedValue = (xValue + 1.0f) * 50.0f;
    m_dashBoard.updateXAxisValue(static_cast<int>(mappedValue));
  });

  m_dashBoardState.accelerometerYValue.attach([this](const float& yValue)
  {
    float mappedValue = (yValue + 1.0f) * 50.0f;
    m_dashBoard.updateYAxisValue(static_cast<int>(mappedValue));
  });

  m_dashBoardState.accelerometerZValue.attach([this](const float& zValue)
  {
    float mappedValue = (zValue + 1.0f) * 50.0f;
    m_dashBoard.updateZAxisValue(static_cast<int>(mappedValue));
  });

  m_dashBoardState.bluetootDeviceConnected.attach([this](const bool& deviceConnected)
  {
    m_dashBoard.updateBluetoothLabel(deviceConnected);
  });
}

void MainApp::lv_tick_task(void* arg)
{
  lv_tick_inc(1);
}