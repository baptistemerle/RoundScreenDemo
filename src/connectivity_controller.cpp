#include "connectivity_controller.h"

#include "ble_controller.h"

ConnectivityController::ConnectivityController(BLEController& ble)
  : m_ble(ble)
{
  m_ble.attachListener(this);
}

void ConnectivityController::onConnected()
{
}

void ConnectivityController::onDisconnected()
{
  m_ble.startAdvertising();
}