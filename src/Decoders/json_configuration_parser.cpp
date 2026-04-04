#include "json_configuration_parser.h"

#include "Core/dashboard_state.h"

#include <ArduinoJson.h>

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
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, json);

  if (error)
    return;

  if (doc["limit"].is<int>())
  {
    int limit = doc["limit"];
    m_dashBoardState.limit = limit;
  }
}