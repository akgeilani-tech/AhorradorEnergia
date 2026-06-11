#include <Arduino.h>
#include <WiFi.h>

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

    if
    (
        WiFi.status()
        ==
        WL_CONNECTED
    )
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

// void setup()
// {
//     Serial.begin(115200);

//     delay(3000);

//     Serial.println("BOOT");
// }

// void loop()
// {
//     delay(1000);

//     Serial.println("RUN");
// }