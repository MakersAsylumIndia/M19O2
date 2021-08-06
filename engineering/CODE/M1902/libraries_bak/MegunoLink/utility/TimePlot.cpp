#include "TimePlot.h"

TimePlot::TimePlot( const char *channelName, Print &rDestination )
  : Plot(F("TIMEPLOT"), channelName, rDestination)
{

}

TimePlot::TimePlot( const __FlashStringHelper *channelName, Print &rDestination )
  : Plot(F("TIMEPLOT"), channelName, rDestination)
{

}

TimePlot::TimePlot(Print& rDestination)
  : Plot(F("TIMEPLOT"), (const char*) NULL, rDestination)
{
}


void TimePlot::Run(bool bEnable)
{
  SendDataHeader(F("RUN"));
  m_rDestination.print(bEnable ? '1' : '0');
  SendDataTail();
}

void TimePlot::SetXRange(float fTimespanHours)
{
	SendDataHeader(F("xrange"));
	m_rDestination.print("T|");
	m_rDestination.print(fTimespanHours,5);
	SendDataTail();
}

void TimePlot::Stop()
{
  Run(false);
}

void TimePlot::SendFloatData(const char * seriesName, float yValue, int nDecimalPlaces, const char * seriesProperties/*=NULL*/)
{
  SendDataHeader(F("DATA"));
  m_rDestination.print(seriesName);
  SendSeriesProperties(seriesProperties);
  m_rDestination.print(F("T|"));
  m_rDestination.print(yValue, nDecimalPlaces);
  SendDataTail();
}

void TimePlot::SendFloatData(const char * seriesName, float yValue, int nDecimalPlaces, Colors Color, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendDataHeader(F("DATA"));
  m_rDestination.print(seriesName);
  SendSeriesProperties(Color, Line, uLineWidth, Marker, ax);
  m_rDestination.print(F("T|"));
  m_rDestination.print(yValue, nDecimalPlaces);
  SendDataTail();
}

void TimePlot::SendFloatData(const char * seriesName, float yValue, int nDecimalPlaces, int32_t nColor, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendDataHeader(F("DATA"));
  m_rDestination.print(seriesName);
  SendSeriesProperties(nColor, Line, uLineWidth, Marker, ax);
  m_rDestination.print(F("T|"));
  m_rDestination.print(yValue, nDecimalPlaces);
  SendDataTail();
}

void TimePlot::SendFloatData(const __FlashStringHelper * seriesName, float yValue, int nDecimalPlaces, const char * seriesProperties/*=NULL*/)
{
  SendDataHeader(F("DATA"));
  m_rDestination.print(seriesName);
  SendFloatDataCore(NULL, NULL, yValue, nDecimalPlaces, seriesProperties);

}

void TimePlot::SendFloatData(const __FlashStringHelper * seriesName, float yValue, int nDecimalPlaces, Colors Color, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendDataHeader(F("DATA"));
  m_rDestination.print(seriesName);
  SendFloatDataCore(NULL, NULL, yValue, nDecimalPlaces, Color, 0, false, Line, uLineWidth, Marker, ax);
}

void TimePlot::SendFloatData(const __FlashStringHelper * seriesName, float yValue, int nDecimalPlaces, int32_t nColor, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendDataHeader(F("DATA"));
  m_rDestination.print(seriesName);
  SendFloatDataCore(NULL, NULL, yValue, nDecimalPlaces, Colors::Black, nColor, true, Line, uLineWidth, Marker, ax);
}

void TimePlot::SetCursorPositionToNow(const char* SeriesName)
{
  SendDataHeader(F("C-POSD"));
  m_rDestination.print(SeriesName);
  m_rDestination.print(F("|T"));
  SendDataTail();
}

void TimePlot::SetCursorPositionToNow(const __FlashStringHelper* SeriesName)
{
  SendDataHeader(F("C-POSD"));
  m_rDestination.print(SeriesName);
  m_rDestination.print(F("|T"));
  SendDataTail();
}

void TimePlot::SetCursorPosition(const char* SeriesName, const tm& Time)
{
  SendDataHeader(F("C-POSD"));
  m_rDestination.print(SeriesName);
  m_rDestination.print(F("|"));
  SendTime(Time);
  SendDataTail();
}

void TimePlot::SetCursorPosition(const __FlashStringHelper* SeriesName, const tm& Time)
{
  SendDataHeader(F("C-POSD"));
  m_rDestination.print(SeriesName);
  m_rDestination.print(F("|"));
  SendTime(Time);
  SendDataTail();
}

void TimePlot::SendTime(const tm& Time)
{
  m_rDestination.print(1900 + Time.tm_year);
  m_rDestination.print('-');
  m_rDestination.print(Time.tm_mon + 1);
  m_rDestination.print('-');
  m_rDestination.print(Time.tm_mday);
  m_rDestination.print(' ');
  m_rDestination.print(Time.tm_hour);
  m_rDestination.print(':');
  m_rDestination.print(Time.tm_min);
  m_rDestination.print(':');
  m_rDestination.print(Time.tm_sec);
}

void TimePlot::SendTimeWithSeparator(const tm& Time)
{
  SendTime(Time);
  m_rDestination.print('|');
}

void TimePlot::SendTimeWithSeparator(const time_t& Time)
{
  m_rDestination.print(Time);
  m_rDestination.print('|');
}

void TimePlot::SendFloatData(const char* seriesName, const tm& Time, float yValue, int nDecimalPlaces, const char* seriesProperties/*=NULL*/)
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(&Time, NULL, yValue, nDecimalPlaces, seriesProperties);
}

void TimePlot::SendFloatData(const char* seriesName, const tm& Time, float yValue, int nDecimalPlaces, Colors Color, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(&Time, NULL, yValue, nDecimalPlaces, Color, 0, false, Line, uLineWidth, Marker, ax);
}

void TimePlot::SendFloatData(const char* seriesName, const tm& Time, float yValue, int nDecimalPlaces, int32_t nColor, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(&Time, NULL, yValue, nDecimalPlaces, Colors::Black, nColor, true, Line, uLineWidth, Marker, ax);
}

void TimePlot::SendFloatData(const __FlashStringHelper* seriesName, const tm& Time, float yValue, int nDecimalPlaces, const char* seriesProperties/*=NULL*/)
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(&Time, NULL, yValue, nDecimalPlaces, seriesProperties);
}

void TimePlot::SendFloatData(const __FlashStringHelper* seriesName, const tm& Time, float yValue, int nDecimalPlaces, Colors Color, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(&Time, NULL, yValue, nDecimalPlaces, Color, 0, false, Line, uLineWidth, Marker, ax);
}

void TimePlot::SendFloatData(const __FlashStringHelper* seriesName, const tm& Time, float yValue, int nDecimalPlaces, int32_t nColor, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(&Time, NULL, yValue, nDecimalPlaces, Colors::Black, nColor, true, Line, uLineWidth, Marker, ax);
}

void TimePlot::SendFloatData(const char* seriesName, const time_t& Time, float yValue, int nDecimalPlaces, const char* seriesProperties )
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(NULL, &Time, yValue, nDecimalPlaces, seriesProperties);
}

void TimePlot::SendFloatData(const char* seriesName, const time_t& Time, float yValue, int nDecimalPlaces, Colors Color, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(NULL, &Time, yValue, nDecimalPlaces, Color, 0, false, Line, uLineWidth, Marker, ax);
}

void TimePlot::SendFloatData(const char* seriesName, const time_t& Time, float yValue, int nDecimalPlaces, int32_t nColor, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(NULL, &Time, yValue, nDecimalPlaces, Colors::Black, nColor, true, Line, uLineWidth, Marker, ax);
}

void TimePlot::SendFloatData(const __FlashStringHelper* seriesName, const time_t& Time, float yValue, int nDecimalPlaces, const char* seriesProperties)
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(NULL, &Time, yValue, nDecimalPlaces, seriesProperties);
}

void TimePlot::SendFloatData(const __FlashStringHelper* seriesName, const time_t& Time, float yValue, int nDecimalPlaces, Colors Color, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(NULL, &Time, yValue, nDecimalPlaces, Color, 0, false, Line, uLineWidth, Marker, ax);
}

void TimePlot::SendFloatData(const __FlashStringHelper* seriesName, const time_t& Time, float yValue, int nDecimalPlaces, int32_t nColor, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  SendHeaderCore(seriesName);
  SendFloatDataCore(NULL, &Time, yValue, nDecimalPlaces, Colors::Black, nColor, true, Line, uLineWidth, Marker, ax);
}

void TimePlot::SendFloatDataCore(const tm* pTime, const time_t* pEpoch, float yValue, int nDecimalPlaces, const char* seriesProperties)
{
  SendSeriesProperties(seriesProperties);
  if (pTime != NULL)
  {
    SendTimeWithSeparator(*pTime);
  }
  else if (pEpoch != NULL)
  {
    SendTimeWithSeparator(*pEpoch);
  }
  else
  {
    SendTimeSeparator();
  }
  m_rDestination.print(yValue, nDecimalPlaces);
  SendDataTail();
}

void TimePlot::SendFloatDataCore(const tm* pTime, const time_t* pEpoch, float yValue, int nDecimalPlaces, Colors Color, int32_t nColor, bool bUseIntColor, LineStyle Line, uint8_t uLineWidth, MarkerStyle Marker, Axis ax)
{
  if (bUseIntColor)
  {
    SendSeriesProperties(nColor, Line, uLineWidth, Marker, ax);
  }
  else
  {
    SendSeriesProperties(Color, Line, uLineWidth, Marker, ax);
  }

  if (pTime != NULL)
  {
    SendTimeWithSeparator(*pTime);
  }
  else if (pEpoch != NULL)
  {
    SendTimeWithSeparator(*pEpoch);
  }
  else
  {
    SendTimeSeparator();
  }

  m_rDestination.print(yValue, nDecimalPlaces);
  SendDataTail();

}