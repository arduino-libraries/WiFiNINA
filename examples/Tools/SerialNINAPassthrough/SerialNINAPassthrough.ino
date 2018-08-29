/*
  SerialNINAPassthrough - Use esptool to flash the u-blox NINA (ESP32) module
  Arduino MKR WiFi 1010, Arduino MKR Vidor 4000, and Arduino UNO WiFi Rev.2.
  
  Copyright (c) 2018 Arduino SA. All rights reserved.

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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

unsigned long baud = 115200;

void setup() {
  SerialUSB.begin(baud);
  SerialNina.begin(baud);
  
  pinMode(NINA_GPIO0, OUTPUT);
  pinMode(NINA_RESETN, OUTPUT);
}

void loop() {
  digitalWrite(NINA_RESETN, SerialUSB.rts());
  digitalWrite(NINA_GPIO0, (SerialUSB.dtr() == 0) ? HIGH : LOW);

  while (SerialUSB.available()) {
    uint8_t c = SerialUSB.read();
    SerialNina.write(c);
  }

  while (SerialNina.available()) {
    uint8_t c = SerialNina.read();
    SerialUSB.write(c);
  }

  // check if the USB virtual serial wants a new baud rate
  if (SerialUSB.baud() != baud) {
    baud = SerialUSB.baud();
    SerialNina.begin(baud);
  }
}
