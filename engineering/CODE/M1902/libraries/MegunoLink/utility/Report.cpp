#include "Report.h"

Report::Report(Print& rDestination)
  : MegunoLinkProtocol(F("REPORT"), rDestination)
{
}

void Report::GenerateNow()
{
  SendDataHeader(F("SAVE"));
  SendDataTail();
}

void Report::EnablePeriodicReports(bool bEnable)
{
  SendDataHeader(F("ENABLE"));
  m_rDestination.print(bEnable ? 'T' : 'F');
  SendDataTail();
}
