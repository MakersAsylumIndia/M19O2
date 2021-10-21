#pragma once
#include "MegunoLinkProtocol.h"

class TestReport : public MegunoLinkProtocol
{
public:
  TestReport(Print &rDestination = Serial);

  void ReportResult(int TestId, bool bPass);
  template <class TData>  void ReportResult(int TestId, bool bPass, TData Data);
  
  void ReportResult(const char *pchName, bool bPass);
  void ReportResult(const __FlashStringHelper *pchName, bool bPass);

  template <class TData>  void ReportResult(const char *pchName, bool bPass, TData Data);
  template <class TData>  void ReportResult(const __FlashStringHelper *pchName, bool bPass, TData Data);

  void Pass(const char *pchName);
  void Pass(const __FlashStringHelper *pchName);
  template <class TData>  void Pass(const char *pchName, TData Data);
  template <class TData>  void Pass(const __FlashStringHelper *pchName, TData Data);

  void Fail(const char *pchName);
  void Fail(const __FlashStringHelper *pchName);
  template <class TData>  void Fail(const char *pchName, TData Data);
  template <class TData>  void Fail(const __FlashStringHelper *pchName, TData Data);

  void StartingTest();
  void TestComplete();

private:
  void SendFirstPart(bool bPass, const char * pchName);
  void SendFirstPart(bool bPass, const __FlashStringHelper * pchName);
  void SendFirstPart(bool bPass, int nTestId);
  void SendFirstPart(bool bPass);
};

template<class TData>
inline void TestReport::ReportResult(int TestId, bool bPass, TData Data)
{
  SendFirstPart(bPass, TestId);
  m_rDestination.print('|');
  m_rDestination.print(Data);
  SendDataTail();
}

template<class TData>
inline void TestReport::ReportResult(const char * pchName, bool bPass, TData Data)
{
  SendFirstPart(bPass, pchName);
  m_rDestination.print('|');
  m_rDestination.print(Data);
  SendDataTail();
}

template<class TData>
inline void TestReport::ReportResult(const __FlashStringHelper * pchName, bool bPass, TData Data)
{
  SendFirstPart(bPass, pchName);
  m_rDestination.print('|');
  m_rDestination.print(Data);
  SendDataTail();
}

template<class TData>
inline void TestReport::Pass(const char * pchName, TData Data)
{
  SendFirstPart(true, pchName);
  m_rDestination.print('|');
  m_rDestination.print(Data);
  SendDataTail();
}

template<class TData>
inline void TestReport::Pass(const __FlashStringHelper * pchName, TData Data)
{
  SendFirstPart(true, pchName);
  m_rDestination.print('|');
  m_rDestination.print(Data);
  SendDataTail();
}

template<class TData>
inline void TestReport::Fail(const char * pchName, TData Data)
{
  SendFirstPart(false, pchName);
  m_rDestination.print('|');
  m_rDestination.print(Data);
  SendDataTail();
}

template<class TData>
inline void TestReport::Fail(const __FlashStringHelper * pchName, TData Data)
{
  SendFirstPart(false, pchName);
  m_rDestination.print('|');
  m_rDestination.print(Data);
  SendDataTail();
}
