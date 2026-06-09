#include <Arduino.h>

#include <WiFi.h>

#include "settings.h"
#include "wifi_manager.h"
#include <WiFiClient.h>

extern Settings settings;

// WiFiClass WiFi is defined in WiFi.h
bool WifiManager::isConfigured() {

    if (
        strlen(settings.wifiSSID) == 0
    ) {

        return false;
    }

    if (
        settings.wifiSSID[0] == 0xFF
    ) {

        return false;
    }

    return true;

}

// WiFiClass WiFi is defined in WiFi.h
void WifiManager::startAP() {

    Serial.println(
        "Starting Access Point..."
    );

    WiFi.disconnect();

    delay(1000);

    bool result =
        WiFi.softAP(
            "AquariumSetup",
            "12345678"
        );

    if (!result) {

        Serial.println(
            "AP FAILED"
        );

        return;
    }

    IPAddress ip =
        WiFi.softAPIP();

    Serial.println(
        "AP STARTED"
    );

    Serial.print(
        "AP IP: "
    );

    Serial.println(ip);
}

// WiFiClass WiFi is defined in WiFi.h
bool WifiManager::connect() {

        if (!isConfigured()) {

            Serial.println(
                "WiFi not configured"
            );

            return false;
        }

        Serial.println();
        Serial.println(
            "Connecting WiFi..."
        );

        /*
            Limpiar completamente
            cualquier estado AP previo
        */

        WiFi.softAPdisconnect(true);

        delay(1000);

        /*
            Forzar modo STA
        */

        WiFi.mode(WIFI_STA);

        /*
            Deshabilitar power save
            del CYW43
        */

        //WiFi.setSleep(false);

        delay(1000);

        /*
            Desconectar cualquier
            sesión previa
        */

        WiFi.disconnect();

        delay(1000);

        /*
            Hostname opcional
        */

        WiFi.setHostname(
            "aquarium-controller"
        );

        /*
            Iniciar conexión
        */

        WiFi.begin(
            settings.wifiSSID,
            settings.wifiPassword
        );

        unsigned long startTime =
            millis();

        /*
            Esperar conexión
        */

        while (
            WiFi.status() != WL_CONNECTED
        ) {

            if (
                millis() - startTime >
                15000
            ) {

                Serial.println();
                Serial.println(
                    "WiFi connection timeout"
                );

                return false;
            }

            Serial.print(".");

            /*
                MUY importante
                para Pico W
            */

            yield();

            delay(10);
        }

        /*
            Esperar DHCP/IP válida
        */

        while (
            WiFi.localIP()[0] == 0
        ) {

            if (
                millis() - startTime >
                20000
            ) {

                Serial.println();
                Serial.println(
                    "DHCP timeout"
                );

                return false;
            }

            yield();

            delay(10);
        }

        /*
            Esperar estabilización
            TCP/IP
        */

        delay(1000);

        Serial.println();
        Serial.println(
            "WiFi Connected"
        );

        Serial.print(
            "IP Address: "
        );

        Serial.println(
            WiFi.localIP()
        );

        Serial.print(
            "RSSI: "
        );

        Serial.println(
            WiFi.RSSI()
        );

        return true;

}

// return true if connected to WiFi, false otherwise
bool WifiManager::connected() {

    return WiFi.status() == WL_CONNECTED;

}

bool WifiManager::hasInternet() {

    WiFiClient client;

    Serial.println(
        "Testing internet..."
    );

    // Probar DNS/Internet con conexión a 8.8.8.8:80

    bool ok =
        client.connect(
            "google.com",
            80
        );

    if (ok) {

        Serial.println(
            "Internet OK"
        );

        client.stop();

        return true;
    }

    Serial.println(
        "No internet"
    );

    return false;
}