#include "preferences_controller.h"

#include "dashboard_state.h"

namespace
{
  static const std::string configurationSpaceKey = "configuration";

  static const std::string limitKey = "limit";
  static const int defaultLimitValue = 130;
}

PreferencesController::PreferencesController(DashBoardState& state)
  : m_dashBoardState(state)
{
}

void PreferencesController::init()
{
  m_preferences.begin(configurationSpaceKey.c_str(), false);

  m_dashBoardState.limit.attach([this](const int& newLimit)
  {
    m_preferences.putInt(limitKey.c_str(), newLimit);
  });
}

void PreferencesController::loadValues()
{
  int savedLimit = m_preferences.getInt(limitKey.c_str(), defaultLimitValue);
  m_dashBoardState.limit = savedLimit;
}