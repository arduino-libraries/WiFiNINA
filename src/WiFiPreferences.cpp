// Copyright 2015-2021 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "WiFiPreferences.h"
#include "utility/wifi_drv.h"

using namespace std;

Preferences::Preferences() {

}

Preferences::~Preferences() {
    end();
}

bool Preferences::begin(const char * name, bool readOnly, const char* partition_label){
    WiFiDrv::wifiDriverInit();
    return WiFiDrv::prefBegin(name, readOnly, partition_label);
}

void Preferences::end() {
    WiFiDrv::prefEnd();
}

/*
 * Clear all keys in opened preferences
 * */

bool Preferences::clear() {

    return WiFiDrv::prefClear();
}

/*
 * Remove a key
 * */

bool Preferences::remove(const char * key) {

    return WiFiDrv::prefRemove(key);
}

/*
 * Put a key value
 * */

size_t Preferences::putChar(const char* key, int8_t value) {
    return WiFiDrv::prefPut(key, PT_I8, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putUChar(const char* key, uint8_t value) {
    return WiFiDrv::prefPut(key, PT_U8, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putShort(const char* key, int16_t value) {
    return WiFiDrv::prefPut(key, PT_I16, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putUShort(const char* key, uint16_t value) {
    return WiFiDrv::prefPut(key, PT_U16, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putInt(const char* key, int32_t value) {
    return WiFiDrv::prefPut(key, PT_I32, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putUInt(const char* key, uint32_t value) {
    return WiFiDrv::prefPut(key, PT_U32, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putLong(const char* key, int32_t value) {
    return WiFiDrv::prefPut(key, PT_I32, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putULong(const char* key, uint32_t value) {
    return WiFiDrv::prefPut(key, PT_U32, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putLong64(const char* key, int64_t value) {
    return WiFiDrv::prefPut(key, PT_I64, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putULong64(const char* key, uint64_t value) {
    return WiFiDrv::prefPut(key, PT_U64, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putFloat(const char* key, const float_t value) {
    return WiFiDrv::prefPut(key, PT_BLOB, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putDouble(const char* key, const double_t value) {
    return WiFiDrv::prefPut(key, PT_BLOB, (uint8_t*) &value, sizeof(value));
}

size_t Preferences::putBool(const char* key, const bool value) {
    return putUChar(key, (uint8_t) (value ? 1 : 0));
}

size_t Preferences::putString(const char* key, const char* value) {
    return WiFiDrv::prefPut(key, PT_STR, (uint8_t*) value, strlen(value)+1); // we also send the \0
}

size_t Preferences::putString(const char* key, const String value) {
    return putString(key, value.c_str());
}

size_t Preferences::putBytes(const char* key, const void* value, size_t len) {
    return WiFiDrv::prefPut(key, PT_BLOB, (uint8_t*) value, len);
}

PreferenceType Preferences::getType(const char* key) {
    return WiFiDrv::prefGetType(key);
}

bool Preferences::isKey(const char* key) {
    return getType(key) != PT_INVALID;
}

/*
 * Get a key value
 * */

int8_t Preferences::getChar(const char* key, const int8_t defaultValue) {
    int8_t res=0;

    if(!isKey(key)) {
        return defaultValue;
    }

    WiFiDrv::prefGet(key, PT_I8, (uint8_t*)&res, sizeof(res));

    return res;
}

uint8_t Preferences::getUChar(const char* key, const uint8_t defaultValue) {
    uint8_t res=0;

    if(!isKey(key)) {
        return defaultValue;
    }

    WiFiDrv::prefGet(key, PT_U8, (uint8_t*)&res, sizeof(res));

    return res;
}

int16_t Preferences::getShort(const char* key, const int16_t defaultValue) {
    int16_t res=0;

    if(!isKey(key)) {
        return defaultValue;
    }

    WiFiDrv::prefGet(key, PT_I16, (uint8_t*)&res, sizeof(res));

    return res;
}

uint16_t Preferences::getUShort(const char* key, const uint16_t defaultValue) {
    uint16_t res=0;

    if(!isKey(key)) {
        return defaultValue;
    }

    WiFiDrv::prefGet(key, PT_U16, (uint8_t*)&res, sizeof(res));

    return res;
}

int32_t Preferences::getInt(const char* key, const int32_t defaultValue) {
    int32_t res=0;

    if(!isKey(key)) {
        return defaultValue;
    }

    WiFiDrv::prefGet(key, PT_I32, (uint8_t*)&res, sizeof(res));

    return res;
}

uint32_t Preferences::getUInt(const char* key, const uint32_t defaultValue) {
    uint32_t res=0;

    if(!isKey(key)) {
        return defaultValue;
    }

    WiFiDrv::prefGet(key, PT_U32, (uint8_t*)&res, sizeof(res));

    return res;
}

int32_t Preferences::getLong(const char* key, const int32_t defaultValue) {

    if(!isKey(key)) {
        return defaultValue;
    }

    return getInt(key, defaultValue);
}

uint32_t Preferences::getULong(const char* key, const uint32_t defaultValue) {

    if(!isKey(key)) {
        return defaultValue;
    }

    return getUInt(key, defaultValue);
}

int64_t Preferences::getLong64(const char* key, const int64_t defaultValue) {
    int64_t res=0;

    if(!isKey(key)) {
        return defaultValue;
    }

    WiFiDrv::prefGet(key, PT_I64, (uint8_t*)&res, sizeof(res));

    return res;
}

uint64_t Preferences::getULong64(const char* key, const uint64_t defaultValue) {
    uint64_t res=0;

    if(!isKey(key)) {
        return defaultValue;
    }

    WiFiDrv::prefGet(key, PT_U64, (uint8_t*)&res, sizeof(res));

    return res;
}

float_t Preferences::getFloat(const char* key, const float_t defaultValue) {
    float_t value = defaultValue;

    if(!isKey(key)) {
        return defaultValue;
    }

    getBytes(key, (void*) &value, sizeof(float_t));
    return value;
}

double_t Preferences::getDouble(const char* key, const double_t defaultValue) {
    double_t value = defaultValue;

    if(!isKey(key)) {
        return defaultValue;
    }

    getBytes(key, (void*) &value, sizeof(double_t));
    return value;
}

bool Preferences::getBool(const char* key, const bool defaultValue) {

    if(!isKey(key)) {
        return defaultValue;
    }

    return getUChar(key, defaultValue ? 1 : 0) == 1;
}

size_t Preferences::getString(const char* key, char* value, const size_t maxLen) {
    return WiFiDrv::prefGet(key, PT_STR, (uint8_t*)value, maxLen);
}

String Preferences::getString(const char* key, const String defaultValue) {
    size_t len = getBytesLength(key);
    char *str = new char[len+1];

    getString(key, str, len+1);
    str[len] = '\0';

    String res(str);
    delete str;
    str = nullptr;

    return res;
}

size_t Preferences::getBytesLength(const char* key) {
    return WiFiDrv::prefLen(key);
}

size_t Preferences::getBytes(const char* key, void * buf, size_t maxLen) {
    return WiFiDrv::prefGet(key, PT_BLOB, (uint8_t*)buf, maxLen);
}

size_t Preferences::freeEntries() {
    return WiFiDrv::prefStat();
}
