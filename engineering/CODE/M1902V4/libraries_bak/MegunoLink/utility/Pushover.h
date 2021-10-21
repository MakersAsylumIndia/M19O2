#pragma once
#include "MegunoLinkProtocol.h"

class Pushover : public MegunoLinkProtocol
{
public:

  enum EPriority
  {
    DefaultPriority = 0,

    LowestPriority = 1,

    LowPriority = 2,

    NormalPriority = 3,

    HighPriority = 4,

    EmergencyPriority = 5,
  };

  enum ENotificationSound
  {
    DefaultSound = 0, // will leave empty on sending
    PushoverSound = 1,
    BikeSound = 2,
    BugleSound = 3,
    CashRegisterSound = 4,
    ClassicalSound = 5,
    CosmicSound = 6,
    FallingSound = 7,
    GamelanSound = 8,
    IncomingSound = 9,
    IntermissionSound = 10,
    MagicSound = 11,
    MechanicalSound = 12,
    PianobarSound = 13,
    SirenSound = 14,
    SpaceAlarmSound = 15,
    TugboatSound = 16,
    AlienSound = 17,
    ClimbSound = 18,
    PersistentSound = 19,
    EchoSound = 20,
    UpdownSound = 21,
    NoSound = 22,
  };

public:
  Pushover(Print& rDestination = Serial);

  template<typename TContent> void Send(TContent Content)
  {
    SendDataHeader(F("SND"));
    m_rDestination.print(Content);
    SendDataTail();
  }

  template<typename TContent> void Send(TContent Content, ENotificationSound Sound)
  {
    SendDataHeader(F("SND"));
    m_rDestination.print(Content);
    m_rDestination.print('|');
    m_rDestination.print((int)Sound);
    SendDataTail();
  }

  template<typename TContent> void Send(TContent Content, ENotificationSound Sound, EPriority Priority)
  {
    SendDataHeader(F("SND"));
    m_rDestination.print(Content);
    m_rDestination.print('|');
    m_rDestination.print((int)Sound);
    m_rDestination.print('|');
    m_rDestination.print((int)Priority);
    SendDataTail();
  }

private:
};