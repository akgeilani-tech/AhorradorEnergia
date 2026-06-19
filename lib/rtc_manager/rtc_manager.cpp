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

DateTime RTCManager::getDateTime()
{
    return rtc.now();
}

uint32_t RTCManager::getUnixTime()
{
    return rtc.now().unixtime();
}

bool RTCManager::setDateTime(DateTime dt)
{
    rtc.adjust(
        dt
    );
    if (rtc.lostPower()) {
            // Si sigue marcando pérdida de energía tras el adjust, la escritura falló
            return false;
        }
    DateTime check = rtc.now();
    if (check.year() < 2020) { 
        // Si devuelve un año absurdo (como el año 2000 por defecto del chip)
        // o falló la lectura I2C, retornamos false
        return false;
    }

return fabs(
    check.unixtime() -
    dt.unixtime()
) < 5;
}

bool RTCManager::isValid()
{
    DateTime now =
        rtc.now();

    return
        now.year()
        >=
        2024;
}