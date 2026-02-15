#include "dashboard_ui.h"

DashBoardUI::~DashBoardUI()
{
  if (m_mainContainer)
  {
    lv_obj_del(m_mainContainer);
    m_mainContainer = nullptr;
    m_gaugeArc = nullptr;
    m_label = nullptr;
  }
}

void DashBoardUI::init(lv_disp_t* screen)
{
  m_mainContainer = lv_obj_create(lv_disp_get_scr_act(screen));
  lv_obj_set_size(m_mainContainer, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_border_width(m_mainContainer, 0, 0);
  lv_obj_set_style_pad_all(m_mainContainer, 0, 0);

  m_gaugeArc = lv_arc_create(m_mainContainer);
  lv_obj_set_size(m_gaugeArc, 220, 220);
  lv_obj_center(m_gaugeArc);
  lv_arc_set_range(m_gaugeArc, 0, 100);

  m_label = lv_label_create(m_mainContainer);
  lv_obj_set_style_text_font(m_label, &lv_font_montserrat_14, 0);
  lv_obj_align(m_label, LV_ALIGN_CENTER, 0, 20);
}

void DashBoardUI::updateArc(int valuePercent)
{
  lv_arc_set_value(m_gaugeArc, valuePercent);
}

void DashBoardUI::updateLabel(std::string text)
{
  lv_label_set_text(m_label, text.c_str());
}