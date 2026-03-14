#ifndef DASHBOARD_STATE_H
#define DASHBOARD_STATE_H

#include "observable_value.h"

struct DashBoardState
{
  ObservableValue<int> limit { 0 };
  ObservableValue<int> arcValue { 0 };
};

#endif // DASHBOARD_STATE_H