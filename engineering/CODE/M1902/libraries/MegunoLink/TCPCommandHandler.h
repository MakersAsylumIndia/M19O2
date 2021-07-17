#pragma once
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)

#include <Arduino.h>
#include <Stream.h>

#include "utility/CommandDispatcherBase.h"
#include "utility/TCPConnection.h"

template<int MAX_CONNECTIONS = 1, int MAX_COMMANDS = 10, int CP_SERIAL_BUFFER_SIZE = 30, int MAX_VARIABLES = 10>
class TCPCommandHandler
  : public MLP::CommandDispatcherBase
  , public Print
{
  // Array of commands we can match & dispatch. 
  MLP::CommandCallback m_Commands[MAX_COMMANDS];

  // Array of variables we can match & set/print
  MLP::VariableMap m_Variables[MAX_VARIABLES];

  // Active TCP connections.
  MLP::TCPConnection<CP_SERIAL_BUFFER_SIZE> m_Connections[MAX_CONNECTIONS];

  // Server that supplies connections. 
  WiFiServer &m_rServer;

public:

  TCPCommandHandler(WiFiServer &rServer, char chStartOfMessage = '!', char chEndOfMessage = '\r')
    : CommandDispatcherBase(m_Commands, MAX_COMMANDS, m_Variables, MAX_VARIABLES)
    , m_rServer(rServer)
  {
    for (int iConnection = 0; iConnection < MAX_CONNECTIONS; ++iConnection)
    {
      m_Connections[iConnection].Init(*this, chStartOfMessage, chEndOfMessage);

    }
  }

  void Process()
  {
    // Check for new clients.
    if (m_rServer.hasClient())
    {
      bool bConnected = false;

      for (int iConnection = 0; iConnection < MAX_CONNECTIONS; ++iConnection)
      {
        MLP::TCPConnection<CP_SERIAL_BUFFER_SIZE> *pConnection = m_Connections + iConnection;
        Serial.print("Chec: "); Serial.println(iConnection);
        if (!pConnection->m_Client || !pConnection->m_Client.connected())
        {
          if (pConnection->m_Client)
          {
            pConnection->m_Client.stop();
          }
          pConnection->m_Client = m_rServer.available();
          pConnection->SetStream(&pConnection->m_Client);
          bConnected = true;
          Serial.print("New connection: ");
          Serial.println(iConnection);
          break;
        }
      }

      // no free/ disconnected client, so reject connection. 
      if (!bConnected)
      {
        Serial.println("Connection rejected");
        m_rServer.available().stop();
      }
    }

    // Process connected clients.
    for (int iConnection = 0; iConnection < MAX_CONNECTIONS; ++iConnection)
    {
      MLP::TCPConnection<CP_SERIAL_BUFFER_SIZE> *pConnection = m_Connections + iConnection;
      if (pConnection->m_Client && pConnection->m_Client.connected())
      {
        m_Connections[iConnection].Process();
      }
    }
  }

  virtual size_t write(uint8_t uValue)
  {
    bool bAnyWritten = false;  
    for (int iConnection = 0; iConnection < MAX_CONNECTIONS; ++iConnection)
    {
      MLP::TCPConnection<CP_SERIAL_BUFFER_SIZE> *pConnection = m_Connections + iConnection;
      if (pConnection->m_Client && pConnection->m_Client.connected())
      {
        m_Connections[iConnection].m_Client.write(uValue);
        bAnyWritten = true;
      }
    }
    return bAnyWritten ? sizeof(uValue) : 0;
  }

  virtual size_t write(const uint8_t *buffer, size_t size)
  {
    bool bAnyWritten = false;  
    
    for (int iConnection = 0; iConnection < MAX_CONNECTIONS; ++iConnection)
    {
      MLP::TCPConnection<CP_SERIAL_BUFFER_SIZE> *pConnection = m_Connections + iConnection;
      if (pConnection->m_Client && pConnection->m_Client.connected())
      {
        m_Connections[iConnection].m_Client.write(buffer, size);
        bAnyWritten = true;
      }
    }
    
    return bAnyWritten ? size : 0;
  }

};

#endif
