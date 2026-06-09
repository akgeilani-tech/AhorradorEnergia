#include "wifi_manager.h"

#include <WiFi.h>

#include "config.h"
#include "settings.h"

extern Settings settings;

WifiManager wifiManager;

void WifiManager::begin()
{
    if (isConfigured())
    {
        connect();
    }
    else
    {
        startAP();
    }
}

void WifiManager::update()
{
}

bool WifiManager::isConfigured()
{
    return strlen(settings.wifi.ssid) > 0;
}

bool WifiManager::connect()
{
    WiFi.mode(WIFI_STA);

    WiFi.begin(
        settings.wifi.ssid,
        settings.wifi.password
    );

    return true;
}

void WifiManager::startAP()
{
    WiFi.mode(WIFI_AP);

    WiFi.softAP(
        AP_SSID,
        AP_PASSWORD
    );
}

bool WifiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool WifiManager::hasInternet()
{
    return false;
}