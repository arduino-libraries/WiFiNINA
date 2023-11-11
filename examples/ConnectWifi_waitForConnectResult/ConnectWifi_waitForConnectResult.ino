/*
 This example connects to an WiFi network.
 Illustrates the usage of the function 
 waitForConnectResult()
 Then it prints the MAC address of the WiFi module,
 the IP address obtained, and other network details.

 created on 9 Nov 2023
 from Frank Häfele
 */
#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h" 
// ==> please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;  // the WiFi radio's status

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

 // Connect to WPA/WPA2 network:
  auto tic = millis();
  // using non blocking wifi connect method!
  (void) WiFi.connect(ssid, pass);
  while ((WiFi.status()) != WL_CONNECTED) {
   yield();
  }
  
  if ((WiFi.status()) == WL_CONNECTED)  {
    Serial.print(F("\nConnection successful!\n"));
    printCurrentNet();
    printWifiData();
  }
  auto toc = millis();
  Serial.print("=> Connection Time: ");
  Serial.print(toc-tic);
  Serial.print(" ms\n\n");

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

}

void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);
  printCurrentNet();
}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

   // print the encryption type:
   printEncryptionType();
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

void print_wifi_status(int status) {
  if (status == -1) {
    Serial.println("Connection Timeout!");
    return;
  }
  /*
	WL_NO_SHIELD = 255,
  WL_NO_MODULE = WL_NO_SHIELD,
  WL_IDLE_STATUS = 0,
  WL_NO_SSID_AVAIL,
  WL_SCAN_COMPLETED,
  WL_CONNECTED,
  WL_CONNECT_FAILED,
  WL_CONNECTION_LOST,
  WL_DISCONNECTED,
  WL_AP_LISTENING,
  WL_AP_CONNECTED,
  WL_AP_FAILED
  */
  
  switch(status) {
    case 0:
      Serial.println("WL_IDLE_STATUS");
      break;
    case 1:
      Serial.println("WL_NO_SSID_AVAIL");
      break;
    case 2:
      Serial.println("WL_SCAN_COMPLETED");
      break;
    case 3:
      Serial.println("WL_CONNECTED");
      break;
    case 4:
      Serial.println("WL_CONNECT_FAILED");
      break;
    case 5:
      Serial.println("WL_CONNECTION_LOST");
      break;
    case 6:
      Serial.println("WL_DISCONNECTED");
      break;
    case 7:
      Serial.println("WL_AP_LISTENING");
      break;
    case 8:
      Serial.println("WL_AP_CONNECTED");
      break;
    case 9:
      Serial.println("WL_AP_FAILED");
      break;
    case 255:
      Serial.println("WL_NO_MODULE");
      break;
  }
}

void printEncryptionType() {
  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("\tEncryption Type: ");
  switch(encryption) {
    case 2:
      Serial.println("ENC_TYPE_TKIP");
      break;
    case 4:
      Serial.println("ENC_TYPE_CCMP");
      break;
    case 5:
      Serial.println("ENC_TYPE_WEP");
      break;
    case 7:
      Serial.println("ENC_TYPE_NONE");
      break;
    case 8:
      Serial.println("ENC_TYPE_AUTO");
      break;
    case 255:
      Serial.println("ENC_TYPE_UNKNOWN");
      break;
  }
  Serial.println();
}
