#pragma once

class WifiManager
{
public:

    void begin();

    void update();

    bool isConfigured();

    bool connect();

    void startAP();

    bool isConnected();

    bool hasInternet();

};

extern WifiManager wifiManager;