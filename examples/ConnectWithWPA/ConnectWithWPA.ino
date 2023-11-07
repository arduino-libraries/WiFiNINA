/*
 This example connects to an unencrypted WiFi network.
 Then it prints the MAC address of the WiFi module,
 the IP address obtained, and other network details.

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 07 Nov 2023
 by Frank Häfele
 */
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

// ==> please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = MY_SSID;        // your network SSID (name)
char pass[] = MY_PASSWORD;    // your network password (use for WPA, or use as key for WEP)

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println(F("\n=== WIFININA - Wifi example ===\n\n"));

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println(F("Communication with WiFi module failed!"));
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println(F("Please upgrade the firmware"));
  }

  int status = WL_IDLE_STATUS;
  auto tic = millis();

  Serial.print(F("Attempting to connect to WPA SSID: "));
  Serial.println(ssid);
  while ((millis() - tic) < 60000U) {
    Serial.print(". ");
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    if (status == WL_CONNECTED) {
      // you're connected now, so print out the data:
      Serial.print(F("\nYou're connected to the network!\n"));
      printCurrentNet();
      printWifiData();
      break;
    }
  }
  if (status != WL_CONNECTED) {
    while (true) {
      Serial.print(F("\n\tError: Connection failed!"));
      delay(5000);
    }
  }
  auto toc = millis();
  Serial.print("=> Connection Time: ");
  Serial.print(toc-tic);
  Serial.print(" ms\n\n");
}

void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);
  printCurrentNet();
}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("\tIP Address: ");
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("\tMAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("\tSSID:  ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("\tBSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("\tRSSI:  ");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("\tEncryption Type: ");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
