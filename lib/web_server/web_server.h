#pragma once

#include <Arduino.h>
#include <WebServer.h>

class WebServerManager
{
public:

    WebServerManager();

    bool begin();

    void update();

    bool connect();

    void handleNetworkStatus();

    void handleRTC();

private:

    WebServer server;

    void setupRoutes();

    void handleRoot();

    void handleCSS();

    void handleJS();

    void handleStatus();

    void handleWifi();

    void handleRestart();

    void handleFactoryReset();

    void handleNotFound();
};

extern WebServerManager webServerManager;