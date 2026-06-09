#pragma once

#include <RTClib.h>

class RTCManager {

public:

    RTC_DS3231 rtc;

    void begin();

    String getTimeString();

    DateTime now();
};