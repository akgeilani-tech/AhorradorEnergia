#include "rtc_manager.h"

#include <Wire.h>
#include <RTClib.h>

#include "config.h"

RTC_DS3231 rtc;

RTCManager rtcManager;

bool RTCManager::begin()
{
    Wire.begin(
        SDA_PIN,
        SCL_PIN
    );

    return rtc.begin();
}

void RTCManager::update()
{
}