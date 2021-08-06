#pragma once
#include <Arduino.h>
#include "Plot.h"

class XYPlot : public Plot
{

public:
  XYPlot(Print& rDestination = Serial);
  XYPlot(const char *channelName, Print &rDestination = Serial);
  XYPlot(const __FlashStringHelper *channelName, Print &rDestination = Serial);
  void SetXRange(float fXLimMin, float fXlimMax);

  template<class TXData, class TYData> void SendData(const char * seriesName, TXData xValue, TYData yValue, const char * seriesProperties = NULL)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(seriesProperties);
    m_rDestination.print(xValue);
    m_rDestination.print('|');
    m_rDestination.print(yValue);
    SendDataTail();
  }

  template<class TXData, class TYData> void SendData(const char * seriesName, TXData *xValues, TYData *yValues, unsigned NumberOfValues, const char * seriesProperties = NULL)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(seriesProperties);
    while (NumberOfValues--)
    {
      m_rDestination.print(*xValues++);
      m_rDestination.print('|');
      m_rDestination.print(*yValues++);
      if (NumberOfValues != 0)
      {
        m_rDestination.print('|');
      }
    }
    SendDataTail();
  }

  template<class TXData, class TYData> void SendData(const char * seriesName, TXData xValue, TYData yValue, Colors Color, LineStyle Line = Solid, uint8_t uLineWidth = 1, MarkerStyle Marker = Circle, Axis ax = DefaultAxis)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(Color, Line, uLineWidth, Marker, ax);
    m_rDestination.print(xValue);
    m_rDestination.print('|');
    m_rDestination.print(yValue);
    SendDataTail();
  }

  template<class TXData, class TYData> void SendData(const char* seriesName, TXData xValue, TYData yValue, int32_t nColor, LineStyle Line = Solid, uint8_t uLineWidth = 1, MarkerStyle Marker = Circle, Axis ax = DefaultAxis)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(nColor, Line, uLineWidth, Marker, ax);
    m_rDestination.print(xValue);
    m_rDestination.print('|');
    m_rDestination.print(yValue);
    SendDataTail();
  }

  template<class TXData> void SendData(const char * seriesName, TXData xValue, float yValue, int nDecimalPlaces, const char * seriesProperties = NULL)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendData<TXData>(&xValue, &yValue, 1, nDecimalPlaces, seriesProperties);
  }

  template<class TXData> void SendData(const char * seriesName, TXData *xValues, float *yValues, unsigned NumberOfValues, int nDecimalPlaces, const char * seriesProperties = NULL)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendData<TXData>(xValues, yValues, NumberOfValues, nDecimalPlaces, seriesProperties);
  }

  template<class TXData> void SendData(const char* seriesName, TXData xValue, float yValue, int nDecimalPlaces, Colors Color, LineStyle Line = Solid, uint8_t uLineWidth = 1, MarkerStyle Marker = Circle, Axis ax = DefaultAxis)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(Color, Line, uLineWidth, Marker, ax);
    m_rDestination.print(xValue);
    m_rDestination.print('|');
    m_rDestination.print(yValue, nDecimalPlaces);
    SendDataTail();
  }

  template<class TXData> void SendData(const char* seriesName, TXData xValue, float yValue, int nDecimalPlaces, int32_t nColor, LineStyle Line = Solid, uint8_t uLineWidth = 1, MarkerStyle Marker = Circle, Axis ax = DefaultAxis)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(nColor, Line, uLineWidth, Marker, ax);
    m_rDestination.print(xValue);
    m_rDestination.print('|');
    m_rDestination.print(yValue, nDecimalPlaces);
    SendDataTail();
  }

  template<class TXData, class TYData> void SendData(const __FlashStringHelper * seriesName, TXData xValue, TYData yValue, const char * seriesProperties = NULL)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(seriesProperties);
    m_rDestination.print(xValue);
    m_rDestination.print('|');
    m_rDestination.print(yValue);
    SendDataTail();
  }

  template<class TXData, class TYData> void SendData(const __FlashStringHelper * seriesName, TXData xValue, TYData yValue, Colors Color, LineStyle Line = Solid, uint8_t uLineWidth = 1, MarkerStyle Marker = Circle, Axis ax = DefaultAxis)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(Color, Line, uLineWidth, Marker, ax);
    m_rDestination.print(xValue);
    m_rDestination.print('|');
    m_rDestination.print(yValue);
    SendDataTail();
  }

  template<class TXData, class TYData> void SendData(const __FlashStringHelper* seriesName, TXData xValue, TYData yValue, int32_t nColor, LineStyle Line = Solid, uint8_t uLineWidth = 1, MarkerStyle Marker = Circle, Axis ax = DefaultAxis)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(nColor, Line, uLineWidth, Marker, ax);
    m_rDestination.print(xValue);
    m_rDestination.print('|');
    m_rDestination.print(yValue);
    SendDataTail();
  }
  
  template<class TXData> void SendData(const __FlashStringHelper * seriesName, TXData xValue, float yValue, int nDecimalPlaces, const char * seriesProperties = NULL)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(seriesProperties);
    m_rDestination.print(xValue);
    m_rDestination.print('|');
    m_rDestination.print(yValue, nDecimalPlaces);
    SendDataTail();
  }

  template<class TXData, class TYData> void SendData(const __FlashStringHelper * seriesName, TXData *xValues, TYData *yValues, unsigned NumberOfValues, const char * seriesProperties = NULL)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(seriesProperties);
    while (NumberOfValues--)
    {
      m_rDestination.print(*xValues++);
      m_rDestination.print('|');
      m_rDestination.print(*yValues++);
      if (NumberOfValues != 0)
      {
        m_rDestination.print('|');
      }
    }
    SendDataTail();
  }

  template<class TXData> void SendData(const __FlashStringHelper * seriesName, TXData xValue, float yValue, int nDecimalPlaces, Colors Color, LineStyle Line = Solid, uint8_t uLineWidth = 1, MarkerStyle Marker = Circle, Axis ax = DefaultAxis)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(Color, Line, uLineWidth, Marker, ax);
    m_rDestination.print(xValue, nDecimalPlaces);
    m_rDestination.print('|');
    m_rDestination.print(yValue, nDecimalPlaces);
    SendDataTail();
  }

  template<class TXData> void SendData(const __FlashStringHelper * seriesName, TXData xValue, float yValue, int nDecimalPlaces, int32_t nColor, LineStyle Line = Solid, uint8_t uLineWidth = 1, MarkerStyle Marker = Circle, Axis ax = DefaultAxis)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendSeriesProperties(nColor, Line, uLineWidth, Marker, ax);
    m_rDestination.print(xValue, nDecimalPlaces);
    m_rDestination.print('|');
    m_rDestination.print(yValue, nDecimalPlaces);
    SendDataTail();
  }

  template<class TXData> void SendData(const __FlashStringHelper * seriesName, TXData *xValues, float *yValues, unsigned NumberOfValues, int nDecimalPlaces, const char * seriesProperties = NULL)
  {
    SendHeader_Data();
    m_rDestination.print(seriesName);
    SendData<TXData>(xValues, yValues, NumberOfValues, nDecimalPlaces, seriesProperties);
  }

  void SendData(const __FlashStringHelper * seriesName, float *xValues, float *yValues, unsigned NumberOfValues, int nDecimalPlaces, const char * seriesProperties = NULL);
  void SendData(const char * seriesName, float *xValues, float *yValues, unsigned NumberOfValues, int nDecimalPlaces, const char * seriesProperties = NULL);

private:
  void SendData(float *xValues, float *yValues, unsigned NumberOfValues, int nDecimalPlaces, const char * seriesProperties = NULL);
  template<class TXData> void SendData(TXData *xValues, float *yValues, unsigned NumberOfValues, int nDecimalPlaces, const char * seriesProperties = NULL)
  {
    SendSeriesProperties(seriesProperties);
    while (NumberOfValues--)
    {
      m_rDestination.print(*xValues++);
      m_rDestination.print('|');
      m_rDestination.print(*yValues++, nDecimalPlaces);
      if (NumberOfValues != 0)
      {
        m_rDestination.print('|');
      }
    }
    SendDataTail();
  }
};
