#include <ESPmDNS.h>
#include "wifi_manager.h"
#include "config.h"
#include "settings.h"
#include "ntp_manager.h"

WifiManager wifiManager;

bool WifiManager::begin()
{
    WiFi.persistent(false);

    WiFi.setAutoReconnect(false);

    WiFi.setSleep(false);

    WiFi.mode(
        WIFI_AP_STA
    );

    WiFi.onEvent(
        wifiEvent
    );

    state =
        WIFI_IDLE;

    return true;
}

void WifiManager::update()
{
    switch(state)
    {
        case WIFI_IDLE:

            startAP();

            if(isConfigured())
            {
                startSTA();

                state =
                    WIFI_CONNECTING;
            }
            else
            {
                state =
                    WIFI_AP_ONLY;
            }

            break;

        case WIFI_AP_ONLY:

            if(reconnectRequest)
            {
                reconnectRequest =
                    false;

                startSTA();

                state =
                    WIFI_CONNECTING;
            }

            break;

        case WIFI_CONNECTING:

            if(gotIpEvent)
            {
                gotIpEvent =
                    false;

                stopAP();

                state =
                    WIFI_CONNECTED;
                
                ntpManager.syncRTC();

                Serial.println(
                    "WiFi Connected"
                );

                Serial.print(
                    "IP: "
                );

                Serial.println(
                    WiFi.localIP()
                );

                if
                (
                    !MDNS.begin(
                        settings.system.hostname
                    )
                )
                {
                    Serial.println(
                        "mDNS Error"
                    );
                    Serial.print(
                        settings.system.hostname
                    );
                }
                else
                {
                    MDNS.addService(
                        "http",
                        "tcp",
                        80
                    );
                    Serial.print(
                        "mDNS: http://"
                    );

                    Serial.print(
                        settings.system.hostname
                    );

                    Serial.println(
                        ".local"
                    );
                }
            
                break;
            }

            if
            (
                millis()
                -
                connectTimer
                >
                WIFI_CONNECT_TIMEOUT
            )
            {
                stopSTA();

                state =
                    WIFI_AP_ONLY;

                Serial.println(
                    "Connection Timeout"
                );
            }

            break;

        case WIFI_CONNECTED:

            if(disconnectEvent)
            {
                disconnectEvent =
                    false;

                MDNS.end();

                startAP();

                startSTA();

                state =
                    WIFI_CONNECTING;
                
                Serial.println(
                    "WiFi Lost"
                );
            }

            if(reconnectRequest)
            {
                reconnectRequest =
                    false;

                startAP();

                stopSTA();

                startSTA();

                state =
                    WIFI_CONNECTING;
            }

            break;

        default:

            break;
    }
}

void WifiManager::wifiEvent(
    WiFiEvent_t event,
    WiFiEventInfo_t info
)
{
    switch(event)
    {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:

            wifiManager.gotIpEvent =
                true;

            break;

        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:

            wifiManager.disconnectEvent =
                true;

            wifiManager.staRunning =
                false;

            break;

        default:

            break;
    }
}

void WifiManager::startAP()
{
    if(apRunning)
    {
        return;
    }

    Serial.println(
        "Starting AP"
    );

    if
    (
        WiFi.softAP(
            AP_SSID,
            AP_PASSWORD
        )
    )
    {
        apRunning =
            true;

        Serial.print(
            "AP IP: "
        );

        Serial.println(
            WiFi.softAPIP()
        );
    }
}

void WifiManager::stopAP()
{
    if(!apRunning)
    {
        return;
    }

    WiFi.softAPdisconnect(
        true
    );

    apRunning =
        false;

    Serial.println(
        "AP OFF"
    );
}

void WifiManager::startSTA()
{
    if(staRunning)
    {
        return;
    }

    if(!isConfigured())
    {
        return;
    }

    staRunning =
        true;

    connectTimer =
        millis();

    Serial.println(
        "Connecting STA"
    );
    
    WiFi.setHostname(
        settings.system.hostname
    );

    Serial.println(WiFi.getHostname());

    WiFi.begin(
        settings.wifi.ssid,
        settings.wifi.password
    );
}

void WifiManager::stopSTA()
{
    WiFi.disconnect(
        false,
        false
    );

    staRunning =
        false;

    gotIpEvent =
        false;

    disconnectEvent =
        false;
}

void WifiManager::requestReconnect()
{
    reconnectRequest =
        true;
}

bool WifiManager::isConfigured()
{
    return
        strlen(
            settings.wifi.ssid
        )
        >
        0;
}

bool WifiManager::isConnected()
{
    return
        state
        ==
        WIFI_CONNECTED;
}

bool WifiManager::hasInternet()
{
    return
        isConnected();
}

WifiState WifiManager::getState()
{
    return
        state;
}

String WifiManager::getCurrentMode()
{
    if(isConnected())
    {
        return "STA";
    }

    return "AP";
}

String WifiManager::getCurrentSSID()
{
    if(isConnected())
    {
        return WiFi.SSID();
    }

    return AP_SSID;
}

String WifiManager::getCurrentIP()
{
    if(isConnected())
    {
        return WiFi.localIP().toString();
    }

    return WiFi.softAPIP().toString();
}