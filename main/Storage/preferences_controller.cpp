#include "preferences_controller.h"

#include "Core/dashboard_state.h"

namespace
{
  static const char* configurationSpaceKey = "configuration";

  static const char* limitKey = "limit";
  static const int defaultLimitValue = 130;
}

PreferencesController::PreferencesController(DashBoardState& state)
  : m_dashBoardState(state)
{
}

void PreferencesController::init()
{
  esp_err_t err = nvs_open(configurationSpaceKey, NVS_READWRITE, &m_nvsHandle);
  if (err != ESP_OK)
    return;

  m_isInitialized = true;

  m_dashBoardState.limit.attach([this](const int& newLimit)
  {
    if (!m_isInitialized)
      return;

    esp_err_t writeError = nvs_set_i32(m_nvsHandle, limitKey, static_cast<int32_t>(newLimit));
    if (writeError == ESP_OK)
    {
      nvs_commit(m_nvsHandle);
    }
  });
}

void PreferencesController::loadValues()
{
  if (!m_isInitialized)
  {
    m_dashBoardState.limit = static_cast<int>(defaultLimitValue);
    return;
  }

  int32_t savedLimit = defaultLimitValue;

  esp_err_t err = nvs_get_i32(m_nvsHandle, limitKey, &savedLimit);

  if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
  {
    // Log error ?
  }

  m_dashBoardState.limit = static_cast<int>(savedLimit);
}