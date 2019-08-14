/*
  Web client using WPA2 Enterprise connection

  This sketch connects to a website (http://www.arduino.cc)
  using the WiFi module.

  This example is written for a network using WPA2 Enterprise encryption.

  Circuit:
   Board with NINA module (Arduino MKR WiFi 1010, MKR VIDOR 4000 and UNO WiFi Rev.2) with firmware > 1.2.4

  created 13 July 2010
  by dlf (Metodo2 srl)
  modified 31 May 2012
  by Tom Igoe
  modified 14 Aug 2019
  by Martino Facchin
*/


#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
char server[] = "www.arduino.cc";    // name address for Arduino.cc (using DNS)

// Initialize the WiFi Client
WiFiClient client;

// WPA2 Enterprise usage:

// Certificates and credentials are stored in secret tab to avoid sharing them.
// For some networks (like eduroam) your configuration will simply look like
//
// #define SECRET_SSID     "eduroam"
// #define SECRET_USERNAME "identity@youruniversity.domain"
// #define SECRET_PASS     "yourpassword"
//
// Call WiFi.beginEnterprise(SECRET_SSID, SECRET_USERNAME, SECRET_PASS) and you should easily connect
//
// In case of more complicated networks (that require a certificate, for example) you can use
// WPA2Enterprise functions
// WPA2Enterprise.addCACertificate() , addClientCertificate(), addIdentity() ...
// and then call WiFi.begin() as usual.

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Configure the wifi module to use provided WPA2 Enterprise parameters (if needed)
  Serial.println("Configuring WPA2 Enterprise credentials");
  WPA2Enterprise.addCACertificate(ca_pem);
  WPA2Enterprise.addClientCertificate(client_crt, client_key);

  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // Don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // Attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA2 enterptise network (shows as an open network).
    status = WiFi.beginEnterprise(ssid, SECRET_USERNAME, SECRET_PASSWORD);

    // Wait 1 second for connection:
    delay(1000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // If you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /asciilogo.txt HTTP/1.1");
    client.println("Host: www.arduino.cc");
    client.println("Connection: close");
    client.println();
  }
}

void loop() {
  // If there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // If the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // Do nothing forevermore:
    while (true);
  }
}


void printWifiStatus() {
  // Print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
