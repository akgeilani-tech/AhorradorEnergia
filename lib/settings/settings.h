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

struct Settings
{
    uint16_t version;

    WifiSettings wifi;

    SystemSettings system;
};

extern Settings settings;