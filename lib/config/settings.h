#pragma once
#define SETTINGS_MAGIC 0x20260509

struct Settings
{
    char wifiSSID[32];

    char wifiPassword[64];

    char hostname[32];
};