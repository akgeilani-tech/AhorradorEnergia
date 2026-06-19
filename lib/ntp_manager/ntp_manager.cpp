#include "ntp_manager.h"

#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include "config.h"
#include "settings.h"
#include "rtc_manager.h"
#include "storage_manager.h"
#include "wifi_manager.h"

NTPManager ntpManager;

static WiFiUDP ntpUDP;

bool NTPManager::begin()
{
    lastCheck =
        0;

    return true;
}

void NTPManager::update()
{
    if
    (
        !settings.ntp.autoSync
    )
    {
        return;
    }

    if
    (
        !wifiManager.isConnected()
    )
    {
        return;
    }

    if
    (
        millis()
        -
        lastCheck
        <
        SYNC_INTERVAL
    )
    {
        return;
    }

    lastCheck =
        millis();

    syncRTC();
    return;
}

bool NTPManager::syncRTC()
{
    if
    (
        !wifiManager.isConnected()
    )
    {
        return false;
    }

    if
    (
        !wifiManager.hasInternet()
    )
    {
        return false;
    }

    Serial.println(
        "NTP: Starting sync"
    );

    NTPClient ntpClient
    (
        ntpUDP,
        settings.ntp.server,
        0,
        60000
    );

    ntpClient.begin();

    bool ok =
        ntpClient.forceUpdate();

    if(!ok)
    {
        Serial.println(
            "NTP: Update failed"
        );

        ntpClient.end();

        return false;
    }

    unsigned long utcEpoch =
        ntpClient.getEpochTime();

    ntpClient.end();

    if
    (
        utcEpoch < 100000
    )
    {
        Serial.println(
            "NTP: Invalid epoch"
        );

        return false;
    }

    unsigned long localEpoch =
        utcEpoch +
        (
            settings.rtc.utcOffsetMinutes
            * 60
        );

    DateTime dt(
        localEpoch
    );

    if
    (
        !rtcManager.setDateTime(
            dt
        )
    )
    {
        Serial.println(
            "NTP: RTC update failed"
        );

        return false;
    }

    settings.ntp.lastSync =
        localEpoch; //utcEpoch;

    storageManager.save();

    Serial.println(
        "NTP: Sync OK"
    );

    Serial.print(
        "UTC Epoch: "
    );

    Serial.println(
        utcEpoch
    );

    Serial.print(
        "Local Epoch: "
    );

    Serial.println(
        localEpoch
    );

    Serial.print(
        "UTC Offset: "
    );

    Serial.println(
        settings.rtc.utcOffsetMinutes
    );

    Serial.print(
        "DateTime: "
    );

    Serial.print(
        dt.year()
    );

    Serial.print("-");

    Serial.print(
        dt.month()
    );

    Serial.print("-");

    Serial.print(
        dt.day()
    );

    Serial.print(" ");

    Serial.print(
        dt.hour()
    );

    Serial.print(":");

    Serial.print(
        dt.minute()
    );

    Serial.print(":");

    Serial.println(
        dt.second()
    );

    return true;
}