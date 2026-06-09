#pragma once

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
    WifiSettings wifi;
    SystemSettings system;
};

extern Settings settings;