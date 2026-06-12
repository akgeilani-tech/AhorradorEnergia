#include <Wire.h>

#include "rtc_manager.h"
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

DateTime RTCManager::now()
{
    return rtc.now();
}

uint32_t RTCManager::getUnixTime()
{
    return rtc.now().unixtime();
}