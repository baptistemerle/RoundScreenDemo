#include "connectivity_controller.h"

#include "ble_controller.h"
#include "dashboard_state.h"

ConnectivityController::ConnectivityController(BLEController& ble, DashBoardState& dashBoardState)
  : m_ble(ble)
  , m_dashBoardState(dashBoardState)
{
  m_ble.attachListener(this);
}

void ConnectivityController::onConnected()
{
  m_dashBoardState.bluetootDeviceConnected = true;
}

void ConnectivityController::onDisconnected()
{
  m_dashBoardState.bluetootDeviceConnected = false;
  m_ble.startAdvertising();
}