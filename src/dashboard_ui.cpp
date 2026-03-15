#include "dashboard_ui.h"

DashBoardUI::~DashBoardUI()
{
  if (m_mainContainer)
  {
    lv_obj_del(m_mainContainer);
    m_mainContainer = nullptr;

    m_bluetoothLabel = nullptr;
    for (int i = 0; i < 3; i++)
    {
      m_axes[i] = nullptr;
    }
    m_limitLabel = nullptr;
    m_versionLabel = nullptr;
  }
}

void DashBoardUI::init(lv_disp_t* screen)
{
  m_mainContainer = lv_obj_create(lv_disp_get_scr_act(screen));
  lv_obj_set_size(m_mainContainer, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_border_width(m_mainContainer, 0, 0);
  lv_obj_set_style_pad_all(m_mainContainer, 0, 0);

  lv_obj_set_flex_flow(m_mainContainer, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(m_mainContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_set_style_pad_top(m_mainContainer, 20, 0);
  lv_obj_set_style_pad_bottom(m_mainContainer, 10, 0);

  {
    lv_obj_t* topStatusContainer = lv_obj_create(m_mainContainer);
    lv_obj_remove_style_all(topStatusContainer);
    lv_obj_set_size(topStatusContainer, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(topStatusContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(topStatusContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(topStatusContainer, 5, 0);

    lv_obj_t* bluetoothIconLabel = lv_label_create(topStatusContainer);
    lv_label_set_text(bluetoothIconLabel, LV_SYMBOL_BLUETOOTH);

    m_bluetoothLabel = lv_label_create(topStatusContainer);
    lv_label_set_text(m_bluetoothLabel, "Disconnected");
  }

  {
    lv_obj_t* barsContainer = lv_obj_create(m_mainContainer);
    lv_obj_remove_style_all(barsContainer);
    lv_obj_set_width(barsContainer, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(barsContainer, 1);
    lv_obj_set_flex_flow(barsContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(barsContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(barsContainer, 15, 0);

    const char* axesNames[] = {"X", "Y", "Z"};

    for (int i = 0; i < 3; i++)
    {
      lv_obj_t* singleBarContainer = lv_obj_create(barsContainer);
      lv_obj_remove_style_all(singleBarContainer);
      lv_obj_set_width(singleBarContainer, LV_SIZE_CONTENT);
      lv_obj_set_height(singleBarContainer, LV_PCT(100));
      lv_obj_set_flex_flow(singleBarContainer, LV_FLEX_FLOW_COLUMN);
      lv_obj_set_flex_align(singleBarContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
      lv_obj_set_style_pad_row(singleBarContainer, 5, 0);

      m_axes[i] = lv_bar_create(singleBarContainer);
      lv_obj_set_width(m_axes[i], 20);
      lv_obj_set_flex_grow(m_axes[i], 1);
      lv_bar_set_range(m_axes[i], 0, 100);
      lv_bar_set_value(m_axes[i], 0, LV_ANIM_OFF);

      lv_obj_t* axisLabel = lv_label_create(singleBarContainer);
      lv_label_set_text(axisLabel, axesNames[i]);
    }
  }

  {
    lv_obj_t* bottomInfoContainer = lv_obj_create(m_mainContainer);
    lv_obj_remove_style_all(bottomInfoContainer);
    lv_obj_set_size(bottomInfoContainer, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(bottomInfoContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(bottomInfoContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(bottomInfoContainer, 2, 0);

    m_limitLabel = lv_label_create(bottomInfoContainer);
    lv_label_set_text(m_limitLabel, "Limit: 0");

    m_versionLabel = lv_label_create(bottomInfoContainer);
    lv_label_set_text(m_versionLabel, "v0");
  }
}

void DashBoardUI::updateBluetoothLabel(bool isConnected)
{
  lv_label_set_text(m_bluetoothLabel, isConnected ? "Connected" : "Disconnected");
}

void DashBoardUI::updateXAxisValue(int valuePercent)
{
  lv_bar_set_value(m_axes[0], valuePercent, LV_ANIM_OFF);
}

void DashBoardUI::updateYAxisValue(int valuePercent)
{
  lv_bar_set_value(m_axes[1], valuePercent, LV_ANIM_OFF);
}

void DashBoardUI::updateZAxisValue(int valuePercent)
{
  lv_bar_set_value(m_axes[2], valuePercent, LV_ANIM_OFF);
}

void DashBoardUI::updateLimitLabel(int limit)
{
  lv_label_set_text_fmt(m_limitLabel, "Limit : %d", limit);
}

void DashBoardUI::updateVersionLabel(const std::string& text)
{
  lv_label_set_text(m_versionLabel, text.c_str());
}