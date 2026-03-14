#ifndef CONNECTIVITY_CONTROLLER_H
#define CONNECTIVITY_CONTROLLER_H

#include "iconnection_observer.h"

class BLEController;

class ConnectivityController : public IConnectionObserver
{
public:
  ConnectivityController(BLEController& ble);

  virtual void onConnected() override;
  virtual void onDisconnected() override;

private:
  BLEController& m_ble;
};

#endif // CONNECTIVITY_CONTROLLER_H