#include <Arduino.h>

#include <WiFiUdp.h>

#include <NTPClient.h>

#include <RTClib.h>

#include "rtc_manager.h"
#include "ntp_manager.h"

WiFiUDP ntpUDP;

NTPClient timeClient(
    ntpUDP,
    "pool.ntp.org",
    -4 * 3600, // Time offset in seconds (UTC-4)
    60000
);

extern RTCManager rtcManager;

void NTPManager::begin() {

    timeClient.begin();
}

void NTPManager::update() {

    if (!timeClient.update()) {

        Serial.println(
            "NTP update failed"
        );

        return;
    }

    unsigned long epoch =
        timeClient.getEpochTime();

    rtcManager.rtc.adjust(
        DateTime(epoch)
    );

    Serial.println(
        "RTC synchronized with NTP"
    );
}