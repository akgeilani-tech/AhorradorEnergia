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

    uint32_t getLastSync();

    void setLastSync(
        uint32_t value
    );

private:

    uint32_t lastSync = 0;

};

extern RTCManager rtcManager;