#pragma once

/* -----------------------------------------------------------------------------
*  Node Address
*  Use MegunoLink Pro's Id Writer to set this address. Address is 2 alpha characters
*  followed by up to 5 numeric digits
*  Address: 10
*  Format: Melvyn alpha, little-endian
*  ----------------------------------------------------------------------------- */
#define DEFAULT_DEVICE_ID_ADDRESS ((uint8_t*)10)

// Broadcast address for site 0 relays.
const uint32_t BroadcastRelayAddress = 0xffffff8f; 

uint32_t GetDeviceAddress(uint8_t const * pLocation = DEFAULT_DEVICE_ID_ADDRESS);
void WriteMelvynFormatId(uint32_t uId);
