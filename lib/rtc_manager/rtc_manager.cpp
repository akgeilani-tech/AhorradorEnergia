#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "rtc_manager.h"

void RTCManager::begin() {

    Wire.begin(SDA_PIN, SCL_PIN);

    Wire.begin();

    rtc.begin();

    if (rtc.lostPower()) {

        Serial.println(
            "RTC lost power"
        );
    }
}

String RTCManager::getTimeString() {

    DateTime t = rtc.now();

    char buffer[6];

    sprintf(
        buffer,
        "%02d:%02d",
        t.hour(),
        t.minute()
    );

    return String(buffer);
}

DateTime RTCManager::now() {

    return rtc.now();
}