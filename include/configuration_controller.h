#ifndef CONFIGURATION_CONTROLLER_H
#define CONFIGURATION_CONTROLLER_H

#include "length_payload_assembler.h"

class DashBoardState;

class ConfigurationController
{
public:
  ConfigurationController(DashBoardState& dashBoardState);

  void pushNewData(const std::string& data);

private:
  void jsonReady(const std::string& json);

private:
  DashBoardState& m_dashBoardState;
  LengthPayloadAssembler<4> m_payloadAssembler;
};

#endif // CONFIGURATION_CONTROLLER_H