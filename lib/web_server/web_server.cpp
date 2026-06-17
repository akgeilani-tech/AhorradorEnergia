
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "web_server.h"
#include "settings.h"
#include "config.h"
#include "storage_manager.h"
#include "wifi_manager.h"
#include "rtc_manager.h"
#include "ntp_manager.h"

WebServerManager webServerManager;

WebServerManager::WebServerManager()
:
server(80)
{
}

bool WebServerManager::begin()
{
    Serial.println(
        "Mounting LittleFS..."
    );

    if
    (
        !LittleFS.begin(true)
    )
    {
        Serial.println(
            "LittleFS mount failed"
        );

        return false;
    }

    Serial.println(
        "LittleFS mounted"
    );

    setupRoutes();

    server.begin();

    Serial.println(
        "WebServer started"
    );

    return true;
}

void WebServerManager::update()
{
    server.handleClient();
}

void WebServerManager::setupRoutes()
{
    server.on(
        "/",
        HTTP_GET,
        [this]()
        {
            handleRoot();
        }
    );

    server.on(
        "/style.css",
        HTTP_GET,
        [this]()
        {
            handleCSS();
        }
    );

    server.on(
        "/app.js",
        HTTP_GET,
        [this]()
        {
            handleJS();
        }
    );

    server.on(
        "/api/status",
        HTTP_GET,
        [this]()
        {
            handleStatus();
        }
    );

    server.on(
        "/api/network",
        HTTP_GET,
        [this]()
        {
            handleNetworkStatus();
        }
    );

    server.on(
        "/api/rtc",
        HTTP_GET,
        [this]()
        {
            handleRTC();
        }
    );

    server.on(
        "/api/rtc",
        HTTP_POST,
        [this]()
        {
            handleRTCSet();
        }
    );

    server.on(
        "/api/rtc/sync",
        HTTP_POST,
        [this]()
        {
            handleRTCSync();
        }
    );

    server.on(
        "/api/wifi",
        HTTP_GET,
        [this]()
        {
            handleWifiGet();
        }
    );

    server.on(
        "/api/wifi",
        HTTP_POST,
        [this]()
        {
            handleWifi();
        }
    );

    server.on(
        "/api/restart",
        HTTP_POST,
        [this]()
        {
            handleRestart();
        }
    );

    server.on(
        "/api/factoryreset",
        HTTP_POST,
        [this]()
        {
            handleFactoryReset();
        }
    );

    server.onNotFound(
        [this]()
        {
            handleNotFound();
        }
    );
}

void WebServerManager::handleRoot()
{
    File file =
        LittleFS.open(
            "/index.html",
            "r"
        );

    if(!file)
    {
        server.send(
            404,
            "text/plain",
            "index.html not found"
        );

        return;
    }

    server.streamFile(
        file,
        "text/html"
    );

    file.close();
}

void WebServerManager::handleCSS()
{
    File file =
        LittleFS.open(
            "/style.css",
            "r"
        );

    if(!file)
    {
        server.send(
            404,
            "text/plain",
            "style.css not found"
        );

        return;
    }

    server.streamFile(
        file,
        "text/css"
    );

    file.close();
}

void WebServerManager::handleJS()
{
    File file =
        LittleFS.open(
            "/app.js",
            "r"
        );

    if(!file)
    {
        server.send(
            404,
            "text/plain",
            "app.js not found"
        );

        return;
    }

    server.streamFile(
        file,
        "application/javascript"
    );

    file.close();
}

void WebServerManager::handleStatus()
{
    JsonDocument doc;

    doc["system"] =
        "Activo";

    doc["wifi"] =
        wifiManager.isConnected()
        ?
        "Conectado"
        :
        "Desconectado";

    doc["ip"] =
        wifiManager.getCurrentIP();

    doc["hostname"] =
        settings.system.hostname;    

    doc["firmware"] =
        settings.version;

    doc["rtc"] =
        rtcManager.isValid()
        ?
        "OK"
        :
        "Sin sincronizar";  

    doc["internet"] =
        wifiManager.hasInternet()
        ?
        "Disponible"
        :
        "No disponible";

        String json;

    serializeJson(
        doc,
        json
    );

    server.send(
        200,
        "application/json",
        json
    );
}

void WebServerManager::handleNetworkStatus()
{
    JsonDocument doc;

    doc["mode"] =
        wifiManager.getCurrentMode();

    doc["ssid"] =
        wifiManager.getCurrentSSID();

    doc["ip"] =
        wifiManager.getCurrentIP();

    doc["rssi"] =
        wifiManager.getCurrentRSSI();

    String json;

    serializeJson(
        doc,
        json
    );

    server.send(
        200,
        "application/json",
        json
    );
}

void WebServerManager::handleRTC()
{
    DateTime now =
        rtcManager.now();

    JsonDocument doc;

    doc["year"] =
        now.year();

    doc["month"] =
        now.month();

    doc["day"] =
        now.day();

    doc["hour"] =
        now.hour();

    doc["minute"] =
        now.minute();

    doc["second"] =
        now.second();

    doc["unix"] =
        now.unixtime();

    doc["ntpServer"] =
        settings.rtc.ntpServer;

    doc["utcOffset"] =
        settings.rtc.utcOffsetMinutes;

    doc["autoSync"] =
        settings.rtc.autoSync;

    doc["lastSync"] =
        settings.rtc.lastSync;

    String json;

    serializeJson(
        doc,
        json
    );

    server.send(
        200,
        "application/json",
        json
    );
}

void WebServerManager::handleRTCSet()
{
    if
    (
        !server.hasArg(
            "plain"
        )
    )
    {
        server.send(
            400,
            "text/plain",
            "Invalid Request"
        );

        return;
    }

    JsonDocument doc;

    if
    (
        deserializeJson(
            doc,
            server.arg(
                "plain"
            )
        )
    )
    {
        server.send(
            400,
            "text/plain",
            "JSON Error"
        );

        return;
    }

    DateTime dt
    (
        doc["year"],
        doc["month"],
        doc["day"],
        doc["hour"],
        doc["minute"],
        doc["second"]
    );

    rtcManager.setDateTime(
        dt
    );

    String ntp =
        doc["ntpServer"]
        |
        NTP_SERVER;

    ntp.toCharArray(
        settings.rtc.ntpServer,
        sizeof(
            settings.rtc.ntpServer
        )
    );

    settings.rtc.utcOffsetMinutes =
        doc["utcOffset"]
        |
        -240;

    settings.rtc.autoSync =
        doc["autoSync"]
        |
        false;

    storageManager.save();

    server.send(
        200,
        "text/plain",
        "RTC Saved"
    );
}

void WebServerManager::handleRTCSync()
{
    if
    (
        ntpManager.syncRTC()
    )
    {
        server.send(
            200,
            "text/plain",
            "NTP Sync OK"
        );
    }
    else
    {
        server.send(
            500,
            "text/plain",
            "NTP Sync Error"
        );
    }
}

void WebServerManager::handleWifi()
{
    if
    (
        !server.hasArg(
            "plain"
        )
    )
    {
        server.send(
            400,
            "text/plain",
            "Invalid Request"
        );

        return;
    }

    JsonDocument doc;

    if
    (
        deserializeJson(
            doc,
            server.arg(
                "plain"
            )
        )
    )
    {
        server.send(
            400,
            "text/plain",
            "JSON Error"
        );

        return;
    }

    String ssid =
        doc["ssid"]
        |
        "";

    String password =
        doc["password"]
        |
        "";

    String hostname =
        doc["hostname"]
        |
        "";

    hostname.toCharArray(
        settings.system.hostname,
        sizeof(
            settings.system.hostname
        )
    );

    ssid.toCharArray(
        settings.wifi.ssid,
        sizeof(
            settings.wifi.ssid
        )
    );

    password.toCharArray(
        settings.wifi.password,
        sizeof(
            settings.wifi.password
        )
    );

    storageManager.save();

    wifiManager.requestReconnect();

    server.send(
        200,
        "text/plain",
        "Configuration Saved"
    );
}

void WebServerManager::handleWifiGet()
{
    JsonDocument doc;

    doc["ssid"] =
        settings.wifi.ssid;

    doc["hostname"] =
        settings.system.hostname;

    doc["connected"] =
        wifiManager.isConnected();

    doc["currentSSID"] =
        wifiManager.getCurrentSSID();

    doc["ip"] =
        wifiManager.getCurrentIP();

    String json;

    serializeJson(
        doc,
        json
    );

    server.send(
        200,
        "application/json",
        json
    );
}

void WebServerManager::handleRestart()
{
    server.send(
        200,
        "text/plain",
        "Restarting..."
    );

    delay(1000);

    ESP.restart();
}

void WebServerManager::handleFactoryReset()
{
    storageManager.reset();

    server.send(
        200,
        "text/plain",
        "Factory Reset"
    );

    delay(1000);

    ESP.restart();
}

void WebServerManager::handleNotFound()
{
    server.send(
        404,
        "text/plain",
        "404 Not Found"
    );
}