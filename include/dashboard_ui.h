#ifndef DASHBOARD_UI_H
#define DASHBOARD_UI_H

#include <lvgl.h>
#include <string>

class DashBoardUI
{
public:
  ~DashBoardUI();

  void init(lv_disp_t* screen);

  void updateBluetoothLabel(bool isConnected);
  void updateXAxisValue(int valuePercent);
  void updateYAxisValue(int valuePercent);
  void updateZAxisValue(int valuePercent);
  void updateLimitLabel(int limit);
  void updateVersionLabel(const std::string& text);

private:
  lv_obj_t* m_mainContainer = nullptr;

  lv_obj_t* m_bluetoothLabel = nullptr;
  lv_obj_t* m_axes[3] = { nullptr, nullptr, nullptr };
  lv_obj_t* m_limitLabel = nullptr;
  lv_obj_t* m_versionLabel = nullptr;
};

#endif // DASHBOARD_UI_H