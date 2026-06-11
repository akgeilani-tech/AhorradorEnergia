#include "wifi_manager.h"

#include <WiFi.h>

#include "config.h"
#include "settings.h"

extern Settings settings;

WifiManager wifiManager;

void WifiManager::begin()
{
    WiFi.persistent(false);

    WiFi.setAutoReconnect(true);

    if (isConfigured())
    {
        if (!connect())
        {
            startAP();
        }
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
    WiFi.disconnect(true, true);

    delay(200);

    WiFi.mode(WIFI_STA);

    WiFi.begin(
        settings.wifi.ssid,
        settings.wifi.password
    );

    uint32_t start =
        millis();

    while
    (
        WiFi.status() != WL_CONNECTED
        &&
        millis() - start < 15000
    )
    {
        delay(250);

        Serial.print(".");
    }

    Serial.println();

    if (
        WiFi.status()
        ==
        WL_CONNECTED
    )
    {
        Serial.println(
            "WiFi connected"
        );

        Serial.println(
            WiFi.localIP()
        );

        return true;
    }

    Serial.println(
        "WiFi failed"
    );

    return false;
}

void WifiManager::startAP()
{
    WiFi.disconnect(true, true);

    delay(500);

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