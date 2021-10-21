#include "Pushover.h"

Pushover::Pushover(Print& rDestination /*= Serial*/)
  : MegunoLinkProtocol(F("PSH"), rDestination)
{

}
