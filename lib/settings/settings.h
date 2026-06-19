#pragma once
#include <stdint.h>

struct WifiSettings
{
    char ssid[32];
    char password[64];
};

struct SystemSettings
{
    char hostname[32];
};

struct RTCSettings
{
    int16_t utcOffsetMinutes;

    char timezoneName[32];
};

struct NTPSettings
{
    char server[64];
    
    bool autoSync;

    uint32_t lastSync;
};

struct Settings
{
    float version;

    WifiSettings wifi;

    SystemSettings system;

    RTCSettings rtc;

    NTPSettings ntp;
};

extern Settings settings;