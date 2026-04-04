#include "accelerometer_controller.h"

#include "Core/dashboard_state.h"

#include <Interfaces/iaccelerometer.h>

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
    m_dashBoardState.accelerometerXValue = accX;
    m_dashBoardState.accelerometerYValue = accY;
    m_dashBoardState.accelerometerZValue = accZ;
  }
}