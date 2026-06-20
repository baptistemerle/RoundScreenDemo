#ifndef ACCELEROMETER_CONTROLLER_H
#define ACCELEROMETER_CONTROLLER_H

class DashBoardState;
class IAccelerometer;

class AccelerometerController
{
public:
  AccelerometerController(IAccelerometer& accelerometer, DashBoardState& dashBoardState);

  void update();

private:
  IAccelerometer& m_accelerometer;
  DashBoardState& m_dashBoardState;
};

#endif // ACCELEROMETER_CONTROLLER_H