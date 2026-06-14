#pragma once

#include <Arduino.h>

class NTPManager
{
public:

    bool begin();

    void update();

    bool syncRTC();

private:

    unsigned long lastCheck = 0;

    static const unsigned long
        SYNC_INTERVAL =
        86400000UL;

};

extern NTPManager ntpManager;