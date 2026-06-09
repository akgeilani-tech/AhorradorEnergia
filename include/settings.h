#pragma once
#define SETTINGS_MAGIC 0x20260509

struct Settings {

    uint32_t magic;

    char wifiSSID[32];

    char wifiPassword[64];

};