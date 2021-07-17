#include "TestReport.h"

TestReport::TestReport(Print & rDestination)
  : MegunoLinkProtocol(F("TEST"), rDestination)
{
}

void TestReport::ReportResult(int TestId, bool bPass)
{
  SendFirstPart(bPass, TestId);
  SendDataTail();
}

void TestReport::ReportResult(const char * pchName, bool bPass)
{
  SendFirstPart(bPass, pchName);
  SendDataTail();
}

void TestReport::ReportResult(const __FlashStringHelper * pchName, bool bPass)
{
  SendFirstPart(bPass, pchName);
  SendDataTail();
}

void TestReport::Pass(const char * pchName)
{
  SendFirstPart(true, pchName);
  SendDataTail();
}

void TestReport::Pass(const __FlashStringHelper * pchName)
{
  SendFirstPart(true, pchName);
  SendDataTail();
}

void TestReport::Fail(const char * pchName)
{
  SendFirstPart(false, pchName);
  SendDataTail();
}

void TestReport::Fail(const __FlashStringHelper * pchName)
{
  SendFirstPart(false, pchName);
  SendDataTail();
}

void TestReport::StartingTest()
{
  SendDataHeader(F("START"));
  SendDataTail();
}

void TestReport::TestComplete()
{
  SendDataHeader(F("DONE"));
  SendDataTail();
}

void TestReport::SendFirstPart(bool bPass, const char * pchName)
{
  SendFirstPart(bPass);
  m_rDestination.print(pchName);
}

void TestReport::SendFirstPart(bool bPass, const __FlashStringHelper * pchName)
{
  SendFirstPart(bPass);
  m_rDestination.print(pchName);
}

void TestReport::SendFirstPart(bool bPass, int nTestId)
{
  SendFirstPart(bPass);
  m_rDestination.print(nTestId);
}

void TestReport::SendFirstPart(bool bPass)
{
  if (bPass)
  {
    SendDataHeader(F("PASS"));
  }
  else
  {
    SendDataHeader(F("FAIL"));
  }
}
