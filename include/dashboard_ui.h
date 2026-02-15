#ifndef DASHBOARD_UI_H
#define DASHBOARD_UI_H

#include <lvgl.h>
#include <string>

class DashBoardUI
{
public:
  ~DashBoardUI();

  void init(lv_disp_t* screen);
  void updateArc(int valuePercent);
  void updateLabel(std::string text);

private:
  lv_obj_t* m_mainContainer = nullptr;
  lv_obj_t* m_gaugeArc =      nullptr;
  lv_obj_t* m_label =         nullptr;
};

#endif // DASHBOARD_UI_H