#include <Arduino.h>

#include "storage_manager.h"
#include "rtc_manager.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "ntp_manager.h"
#include "settings.h"

void setup()
{
    Serial.begin(115200);

    storageManager.begin();
    storageManager.load();

    rtcManager.begin();

    wifiManager.begin();

    if (wifiManager.isConnected())
    {
        ntpManager.syncRTC();
    }

    webServerManager.begin();
}

void loop()
{
    wifiManager.update();

    rtcManager.update();

    delay(100);
}