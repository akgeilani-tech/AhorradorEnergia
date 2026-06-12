#pragma once

#include <Arduino.h>
#include <WiFi.h>

enum WifiState
{
    WIFI_IDLE,

    WIFI_AP_ONLY,

    WIFI_CONNECTING,

    WIFI_CONNECTED
};

class WifiManager
{
public:

    bool begin();

    void update();

    void requestReconnect();

    bool isConnected();

    bool hasInternet();

    bool isConfigured();

    WifiState getState();

    String getCurrentSSID();

    String getCurrentIP();

    String getCurrentMode();

private:

    static void wifiEvent(
        WiFiEvent_t event,
        WiFiEventInfo_t info
    );

    void startAP();

    void stopAP();

    void startSTA();

    void stopSTA();

    bool apRunning =
        false;

    bool staRunning =
        false;

    bool reconnectRequest =
        false;

    bool gotIpEvent =
        false;

    bool disconnectEvent =
        false;

    uint32_t connectTimer =
        0;

    WifiState state =
        WIFI_IDLE;

    friend class WifiManagerFriend;
};

extern WifiManager wifiManager;