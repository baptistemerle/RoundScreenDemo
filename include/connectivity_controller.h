#ifndef CONNECTIVITY_CONTROLLER_H
#define CONNECTIVITY_CONTROLLER_H

#include "iconnection_observer.h"

class BLEController;
class DashBoardState;

class ConnectivityController : public IConnectionObserver
{
public:
  ConnectivityController(BLEController& ble, DashBoardState& dashBoardState);

  virtual void onConnected() override;
  virtual void onDisconnected() override;

private:
  BLEController& m_ble;
  DashBoardState& m_dashBoardState;
};

#endif // CONNECTIVITY_CONTROLLER_H