#pragma once

class WifiManager {

public:

    bool connect();

    void startAP();

    bool isConfigured();

    bool connected();

    bool hasInternet();
    
};