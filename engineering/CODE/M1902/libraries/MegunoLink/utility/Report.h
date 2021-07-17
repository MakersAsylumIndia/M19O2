#pragma once
#include "MegunoLinkProtocol.h"

class Report : public MegunoLinkProtocol
{

public:
  Report(Print& rDestination = Serial);

  void GenerateNow();

  void EnablePeriodicReports(bool bEnable = true);

};