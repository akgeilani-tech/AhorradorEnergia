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

};

extern NTPManager ntpManager;