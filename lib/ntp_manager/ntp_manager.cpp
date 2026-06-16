#include "ntp_manager.h"

#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

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
        !settings.rtc.autoSync
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
        // Serial.println(
        //     "NTP: WiFi not connected"
        // );

        return false;
    }

    Serial.println(
        "NTP: Starting sync"
    );

    NTPClient ntpClient
    (
        ntpUDP,
        settings.rtc.ntpServer,
        0,
        60000
    );

    ntpClient.begin();

    bool ok =
        ntpClient.forceUpdate();

    if
    (
        !ok
    )
    {
        Serial.println(
            "NTP: Update failed"
        );

        ntpClient.end();

        return false;
    }

    unsigned long epoch =
        ntpClient.getEpochTime();

    ntpClient.end();

    epoch +=
        (
            settings.rtc.utcOffsetMinutes
            *
            60
        );

    DateTime dt(
        epoch
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

    settings.rtc.lastSync =
        epoch;
    
    settings.rtc.autoSync =
        settings.rtc.autoSync;
    
    storageManager.save();

    Serial.println(
        "NTP: Sync OK"
    );

    Serial.print(
        "NTP Server: "
    );

    Serial.println(
        settings.rtc.ntpServer
    );

    Serial.print(
        "Epoch: "
    );

    Serial.println(
        epoch
    );

    Serial.print(
        "DateTime: "
    );

    Serial.print(
        dt.year()
    );

    Serial.print(
        "-"
    );

    Serial.print(
        dt.month()
    );

    Serial.print(
        "-"
    );

    Serial.print(
        dt.day()
    );

    Serial.print(
        " "
    );

    Serial.print(
        dt.hour()
    );

    Serial.print(
        ":"
    );

    Serial.print(
        dt.minute()
    );

    Serial.print(
        ":"
    );

    Serial.println(
        dt.second()
    );

    return true;
}