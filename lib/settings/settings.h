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
    char ntpServer[64];

    int16_t utcOffsetMinutes;

    char timezoneName[32];

    bool autoSync;

    uint32_t lastSync;
};

struct RTCStatus
{
    uint32_t lastSync;

    bool synced;
};

struct Settings
{
    uint16_t version;

    WifiSettings wifi;

    SystemSettings system;

    RTCSettings rtc;
};

extern Settings settings;