#ifndef DASHBOARD_STATE_H
#define DASHBOARD_STATE_H

#include "Patterns/observable_value.h"

struct DashBoardState
{
  ObservableValue<int>   limit { 0 };
  ObservableValue<float> accelerometerXValue { 0 };
  ObservableValue<float> accelerometerYValue { 0 };
  ObservableValue<float> accelerometerZValue { 0 };
  ObservableValue<bool>  bluetootDeviceConnected { false };
};

#endif // DASHBOARD_STATE_H