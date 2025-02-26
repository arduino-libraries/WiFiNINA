#pragma once

#include <Preferences.h>
#include <functional>

// Preferences p;
// Preferences *preferences=&p;
const char KEY[] = "arduino";

template<class T>
bool test_preferences(T value,
    std::function<size_t(const char*, T)> putf,
    std::function<T(const char*)> getf,
    Preferences *preferences) {
  // we are going now to test all the apis of preferences:
  // .1 We check that the key do not exist
  // .2 we put a value inside the KVStore
  // .3 we check that the size returned is correct
  // .4 we check again the key exists
  // .5 we get the value contained
  // .6 we compare the value with what we inserted at the beginning
  // .7 we remove the value

  Serial.println("Tesing isKey()");
  if(preferences->isKey(KEY)) {
    Serial.println("[Error] kvstore already contains a key");
    return false;
  }

  Serial.println("Tesing put()");
  size_t s;
  if((s=putf(KEY, value)) != sizeof(value)) {
    Serial.println("[Error] kvstore put returned a size that is different from the expected one: ");
    Serial.print(s, HEX);
    Serial.print(" != ");
    Serial.println(sizeof(value), HEX);
    return false;
  }

  Serial.println("Tesing isKey()"); // FIXME always failing
  if(!preferences->isKey(KEY)) {
    Serial.println("[Error] The inserted key is not present in the KVStore");
    return false;
  }

  Serial.println("Tesing getf()");
  T val;
  if((val = getf(KEY)) != value) {
    Serial.print("[Error] get of the previously value returned a wrong value: ");
    Serial.print(val, HEX);
    Serial.print(" != ");
    Serial.println(value, HEX);
    return false;
  }

  Serial.println("Tesing remove()");
  if(!preferences->remove(KEY)) {
    Serial.println("[Error] Failed removing the inserted key");
    return false;
  }

  return true;
}

bool test_preferences(char* value, Preferences *preferences) {
  // we are going now to test all the apis of preferences:
  // .1 We check that the key do not exist
  // .2 we put a value inside the KVStore
  // .3 we check that the size returned is correct
  // .4 we check again the key exists
  // .5 we get the value contained
  // .6 we compare the value with what we inserted at the beginning
  // .7 we remove the value

  Serial.println("Tesing isKey()");
  if(preferences->isKey(KEY)) {
    Serial.println("[Error] kvstore already contains a key");
    return false;
  }

  Serial.println("Tesing put()");
  size_t s;
  if((s=preferences->putString(KEY, value)) != strlen(value)) {
    Serial.println("[Error] kvstore put returned a size that is different from the expected one: ");
    Serial.print(s);
    Serial.print(" != ");
    Serial.println(strlen(value));
    return false;
  }

  Serial.println("Tesing isKey()");
  if(!preferences->isKey(KEY)) {
    Serial.println("[Error] The inserted key is not present in the KVStore");
    return false;
  }

  Serial.println("Tesing getf()");
  char val[500];
  preferences->getString(KEY, val, sizeof(val));
  if(strcmp(val, value) != 0) {
    Serial.print("[Error] get of the previously value returned a wrong value: \"");
    Serial.print(val);
    Serial.print("\" != \"");
    Serial.print(value);
    Serial.println("\"");

    for(int i=0; i<strlen(val); i++) {
      Serial.print(i);
      Serial.print(": ");
      Serial.print((uint8_t)val[i]);
      Serial.print(val[i]!=value[i]? "\" != \"" : "\" == \"");
      Serial.print((uint8_t)value[i]);
      Serial.println("\"");
    }

    return false;
  }

  Serial.println("Tesing remove()");
  if(!preferences->remove(KEY)) {
    Serial.println("[Error] Failed removing the inserted key");
    return false;
  }

  return true;
}

bool test_preferences(uint8_t* value, size_t size, Preferences *preferences) {
  // we are going now to test all the apis of preferences:
  // .1 We check that the key do not exist
  // .2 we put a value inside the KVStore
  // .3 we check that the size returned is correct
  // .4 we check again the key exists
  // .5 we get the value contained
  // .6 we compare the value with what we inserted at the beginning
  // .7 we remove the value

  Serial.println("Tesing isKey()");
  if(preferences->isKey(KEY)) {
    Serial.println("[Error] kvstore already contains a key");
    return false;
  }

  Serial.println("Tesing put()");
  size_t s;
  if((s=preferences->putBytes(KEY, value, size)) != size) {
    Serial.println("[Error] kvstore put returned a size that is different from the expected one: ");
    Serial.print(s);
    Serial.print(" != ");
    Serial.println(size);
    return false;
  }

  Serial.println("Tesing isKey()");
  if(!preferences->isKey(KEY)) {
    Serial.println("[Error] The inserted key is not present in the KVStore");
    return false;
  }

  Serial.println("Tesing getBytesLength()");
  if((s=preferences->getBytesLength(KEY)) != size) {
    Serial.println("[Error] The length of the value do not match with the expected one");
    Serial.print(s);
    Serial.print(" != ");
    Serial.println(size+1);
    return false;
  }

  Serial.println("Tesing getf()");
  uint8_t val[500];
  s = preferences->getBytes(KEY, val, sizeof(val));

  if(s != size ) {
    Serial.print("[Error] get of the previously inserted value returned a wrong size: ");
    Serial.print(s);
    Serial.print(" != ");
    Serial.println(size );
    return false;
  }

  if(memcmp(val, value, s) != 0) {
    Serial.println("[Error] get of the previously inserted value returned a wrong value");
    return false;
  }

  Serial.println("Tesing remove()");
  if(!preferences->remove(KEY)) {
    Serial.println("[Error] Failed removing the inserted key");
    return false;
  }

  return true;
}
