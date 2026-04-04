#ifndef PREFERENCES_CONTROLLER_H
#define PREFERENCES_CONTROLLER_H

#include <Preferences.h>

class DashBoardState;

class PreferencesController
{
public:
  PreferencesController(DashBoardState& state);

  void init();
  void loadValues();

private:
    DashBoardState& m_dashBoardState;
    Preferences m_preferences;
};

#endif // PREFERENCES_CONTROLLER_H