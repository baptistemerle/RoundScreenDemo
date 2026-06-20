#ifndef PREFERENCES_CONTROLLER_H
#define PREFERENCES_CONTROLLER_H

#include <nvs.h>

class DashBoardState;

class PreferencesController
{
public:
  PreferencesController(DashBoardState& state);

  void init();
  void loadValues();

private:
    DashBoardState& m_dashBoardState;
    nvs_handle_t    m_nvsHandle;
    bool            m_isInitialized;
};

#endif // PREFERENCES_CONTROLLER_H