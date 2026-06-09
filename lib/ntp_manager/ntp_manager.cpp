#include "ntp_manager.h"

#include <WiFi.h>
#include <time.h>

NTPManager ntpManager;

bool NTPManager::syncRTC()
{
    configTime(
        0,
        0,
        "pool.ntp.org"
    );

    // Obtener tiempo NTP
    // Actualizar DS3231

    return true;
}