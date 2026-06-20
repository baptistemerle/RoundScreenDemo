#ifndef JSON_CONFIGURATION_PARSER_H
#define JSON_CONFIGURATION_PARSER_H

#include <Protocols/length_payload_assembler.hpp>

class DashBoardState;

class JsonConfigurationParser
{
public:
  JsonConfigurationParser(DashBoardState& dashBoardState);

  void pushNewData(const std::string& data);

private:
  void jsonReady(const std::string& json);

private:
  DashBoardState& m_dashBoardState;
  LengthPayloadAssembler<4> m_payloadAssembler;
};

#endif // JSON_CONFIGURATION_PARSER_H