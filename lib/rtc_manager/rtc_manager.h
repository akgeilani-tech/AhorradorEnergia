#pragma once
#include <RTClib.h>

class RTCManager
{
public:

    bool begin();

    void update();

    DateTime now();

    uint32_t getUnixTime();

    bool isValid();

    bool setDateTime(
        DateTime dt
    );

};

extern RTCManager rtcManager;