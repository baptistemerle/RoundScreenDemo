#include "json_configuration_parser.h"

#include "Core/dashboard_state.h"

#include <cJSON.h>

#include <memory>

JsonConfigurationParser::JsonConfigurationParser(DashBoardState& dashBoardState)
  : m_dashBoardState(dashBoardState)
  , m_payloadAssembler([this](const std::string& json) { jsonReady(json); })
{
}

void JsonConfigurationParser::pushNewData(const std::string& data)
{
  m_payloadAssembler.pushData(data);
}

void JsonConfigurationParser::jsonReady(const std::string& json)
{
  std::unique_ptr<cJSON, decltype(&cJSON_Delete)> doc(cJSON_Parse(json.c_str()), cJSON_Delete);

  if (!doc)
    return;

  cJSON* limitItem = cJSON_GetObjectItemCaseSensitive(doc.get(), "limit");

  if (limitItem != nullptr && cJSON_IsNumber(limitItem))
  {
    int limit = limitItem->valueint;
    m_dashBoardState.limit = limit;
  }
}