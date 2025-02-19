/*
  This example 
  created for arduino 06/11/2024
  by Andrea Gilardoni
 */
#include "test.h"

Preferences preferences;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println();

  // Open Preferences with my-app namespace. Each application module, library, etc
  // has to use a namespace name to prevent key name collisions. We will open storage in
  // RW-mode (second parameter has to be false).
  // Note: Namespace name is limited to 15 chars.
  if (!preferences.begin("my-app", false)) {
    Serial.println("Cannot initialize preferences");
    Serial.println("Make sure your WiFi firmware version is greater than 0.3.0");
    while(1) {};
  }

  // Remove all preferences under the opened namespace
  Serial.print("Clearing content of kvstore: ");
  Serial.println(preferences.clear());

  Serial.println("Testing Char operations");
  if(!test_preferences<char>(0x50,
    std::bind(&Preferences::putChar, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getChar(key);
    }, &preferences)) {

    Serial.println("Char test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing UChar operations");
  if(!test_preferences<char>(0x50,
    std::bind(&Preferences::putUChar, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getUChar(key);
    }, &preferences)) {

    Serial.println("UChar test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing Short operations");
  if(!test_preferences<int16_t>(0x5050,
    std::bind(&Preferences::putShort, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getShort(key);
    }, &preferences)) {

    Serial.println("Short test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing UShort operations");
  if(!test_preferences<uint16_t>(0x5050,
    std::bind(&Preferences::putUShort, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getUShort(key);
    }, &preferences)) {

    Serial.println("UShort test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing Int operations");
  if(!test_preferences<int32_t>(0x50505050,
    std::bind(&Preferences::putInt, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getInt(key);
    }, &preferences)) {

    Serial.println("Int test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing UInt operations");
  if(!test_preferences<uint32_t>(0x50505050,
    std::bind(&Preferences::putUInt, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getUInt(key);
    }, &preferences)) {

    Serial.println("UInt test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing Long operations");
  if(!test_preferences<int32_t>(0x50505050,
    std::bind(&Preferences::putLong, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getLong(key);
    }, &preferences)) {

    Serial.println("Long test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing ULong operations");
  if(!test_preferences<uint32_t>(0x50505050,
    std::bind(&Preferences::putULong, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getULong(key);
    }, &preferences)) {

    Serial.println("ULong test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing Long64 operations");
  if(!test_preferences<int64_t>(0x5050505050505050,
    std::bind(&Preferences::putLong64, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getLong64(key);
    }, &preferences)) {

    Serial.println("Long64 test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing ULong64 operations");
  if(!test_preferences<uint64_t>(0x5050505050505050,
    std::bind(&Preferences::putULong64, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getULong64(key);
    }, &preferences)) {

    Serial.println("ULong64 test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing Float operations");
  if(!test_preferences<float>(0x50505050,
    std::bind(&Preferences::putFloat, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getFloat(key);
    }, &preferences)) {

    Serial.println("Float test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing Double operations");
  if(!test_preferences<double>(0x5050505050505050,
    std::bind(&Preferences::putDouble, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getDouble(key);
    }, &preferences)) {

    Serial.println("Double test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing Bool operations");
  if(!test_preferences<bool>(true,
    std::bind(&Preferences::putBool, &preferences, std::placeholders::_1, std::placeholders::_2),
    [&preferences](const char* key) { // we need a lambda, because it requires a default value from apis
      return preferences.getBool(key);
    }, &preferences)) {

    Serial.println("Bool test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing String operations");
  if(!test_preferences("la mia stringsa asdasdafasdjdsnajdnaskjlasda\n\n\r\nsdafasdjdsnajdnaskjlasdasdafasdjdsnajdnaskjlasdasdafasdjdsnajdnaskjlasdasdafasdjdsnajdnaskjlasdasdafasdjdsnajdnaskjl\r\nOK\r\nERROR",
      &preferences)) {
    Serial.println("String test failed");
    preferences.remove(KEY);
  }

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("Testing byte buffer operations");
  uint8_t buffer[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B};
  if(!test_preferences(buffer, sizeof(buffer), &preferences)) {
    Serial.println("byte buffer test failed");
    preferences.remove(KEY);
  }

  Serial.print("\n\nNumber of free entries: ");
  Serial.println(preferences.freeEntries());

  // res = preferences.putString(KEY, String value);      // TODO

  // String getString(const char* key, String defaultValue = String());
  preferences.end();
  Serial.println("\n\nTesting finished");
}

void loop() {}
