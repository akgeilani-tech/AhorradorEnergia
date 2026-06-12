#pragma once
#include <RTClib.h>

class RTCManager
{
public:

    bool begin();

    void update();

    DateTime now();

    uint32_t getUnixTime();

};

extern RTCManager rtcManager;