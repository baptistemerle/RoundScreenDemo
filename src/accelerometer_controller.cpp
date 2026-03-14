#include "accelerometer_controller.h"

#include "iaccelerometer.h"
#include "dashboard_state.h"

#include <Arduino.h>

AccelerometerController::AccelerometerController(IAccelerometer& accelerometer, DashBoardState& dashBoardState)
  : m_accelerometer(accelerometer)
  , m_dashBoardState(dashBoardState)
{
}

void AccelerometerController::update()
{
  float accX, accY, accZ;

  if (m_accelerometer.getValues(accX, accY, accZ))
  {
    int arcValue = map(accX * 100, -100, 100, 0, 100);
    m_dashBoardState.arcValue = arcValue;
  }
}