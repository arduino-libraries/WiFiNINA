/*
  This file is part of the WiFiNINA library.
  Copyright (c) 2021 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifdef ARDUINO_NANO_RP2040_CONNECT

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "nina_pins.h" /* variants/NANO_RP2040_CONNECT/ninaPins.h */
#include "wifi_drv.h"

/******************************************************************************
 * FUNCTION DEFINITION
 ******************************************************************************/

void pinMode(NinaPin pin, PinMode mode)
{
  WiFiDrv::wifiDriverInit();
  Serial.print(__FUNCTION__);
  Serial.print(": pin = ");
  Serial.print(pin);
  Serial.print(", mode = ");
  Serial.println(mode);
  WiFiDrv::pinMode(static_cast<uint8_t>(pin), static_cast<uint8_t>(mode));
}

PinStatus digitalRead(NinaPin pin)
{
  WiFiDrv::wifiDriverInit();
  return WiFiDrv::digitalRead(static_cast<uint8_t>(pin));
}

void digitalWrite(NinaPin pin, PinStatus value)
{
  WiFiDrv::wifiDriverInit();
  Serial.print(__FUNCTION__);
  Serial.print(": pin = ");
  Serial.print(pin);
  Serial.print(", value = ");
  Serial.println(value);
  WiFiDrv::digitalWrite(static_cast<uint8_t>(pin), static_cast<uint8_t>(value));
}

int analogRead(NinaPin pin)
{
  WiFiDrv::wifiDriverInit();
  return WiFiDrv::analogRead(static_cast<uint8_t>(pin));
}

void analogWrite(NinaPin pin, int value)
{
  WiFiDrv::wifiDriverInit();
  WiFiDrv::analogWrite(static_cast<uint8_t>(pin), static_cast<uint8_t>(value));
}

#endif /* ARDUINO_NANO_RP2040_CONNECT */
