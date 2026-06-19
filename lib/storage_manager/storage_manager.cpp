#include "storage_manager.h"

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "config.h"
#include "settings.h"

StorageManager storageManager;

bool StorageManager::begin()
{
    Serial.println("Mounting LittleFS...");

    if (!LittleFS.begin(true))
    {
        Serial.println("LittleFS mount failed");

        return false;
    }

    Serial.println("LittleFS mounted");

    return true;
}

bool StorageManager::createDefault()
{
    Serial.println("Creating default configuration");

    settings.version = SETTINGS_VERSION;

    strcpy(
        settings.wifi.ssid,
        ""
    );

    strcpy(
        settings.wifi.password,
        ""
    );

    strcpy(
        settings.system.hostname,
        HOSTNAME
    );

    settings.rtc.utcOffsetMinutes =
        0;

    strlcpy(
        settings.rtc.timezoneName,
        "GMT Standard Time",
        sizeof(settings.rtc.timezoneName)
    );

    strcpy(
        settings.ntp.server,
        NTP_SERVER
    );

    settings.ntp.autoSync =
        true;

    settings.ntp.lastSync =
        false;
        
    return save();
}

bool StorageManager::load()
{
    // VErificar si el archivo existe, si no crear uno por defecto
    if (!LittleFS.exists(CONFIG_FILE))
    {
        Serial.println("Configuration file not found");

        return createDefault();
    }

    File file =
        LittleFS.open(
            CONFIG_FILE,
            "r"
        );
    // Verificar si se pudo abrir el archivo
    if (!file)
    {
        Serial.println("Cannot open configuration file");

        return false;
    }
    // Deserializar el JSON
    JsonDocument doc;

    DeserializationError error =
        deserializeJson(
            doc,
            file
        );

    file.close();
        
    // Verificar si hubo error al deserializar
    if (error)
    {
        Serial.println("Invalid JSON");

        return createDefault();
    }

    float fileVersion =
        doc["version"] |
        0;

    if
    (
        fileVersion
        !=
        SETTINGS_VERSION
    )
    {
        Serial.println(
            "Configuration version mismatch"
        );

        return createDefault();
    }

    settings.version =
        doc["version"] |
        SETTINGS_VERSION;

    strlcpy(
        settings.wifi.ssid,
        doc["wifi"]["ssid"] |
        "",
        sizeof(settings.wifi.ssid)
    );

    strlcpy(
        settings.wifi.password,
        doc["wifi"]["password"] |
        "",
        sizeof(settings.wifi.password)
    );

    strlcpy(
        settings.system.hostname,
        doc["system"]["hostname"] |
        HOSTNAME,
        sizeof(settings.system.hostname)
    );

    settings.rtc.utcOffsetMinutes =
        doc["rtc"]["utcOffsetMinutes"] |
        0;

    strlcpy(
        settings.rtc.timezoneName,
        doc["rtc"]["timezoneName"] |
        "GMT Standard Time",
        sizeof(settings.rtc.timezoneName)
    );

    strlcpy(
        settings.ntp.server,
        doc["ntp"]["server"] |
        NTP_SERVER,
        sizeof(settings.ntp.server)
    );

    settings.ntp.lastSync =
        doc["ntp"]["lastSync"] |
        false;

    settings.ntp.autoSync =
        doc["ntp"]["autoSync"] |
        false;

    Serial.println("Configuration loaded");

    return true;
}

bool StorageManager::save()
{
    File file =
        LittleFS.open(
            CONFIG_FILE,
            "w"
        );

    if (!file)
    {
        Serial.println("Cannot create configuration file");

        return false;
    }

    JsonDocument doc;

    doc["version"] =
        settings.version;

    doc["wifi"]["ssid"] =
        settings.wifi.ssid;

    doc["wifi"]["password"] =
        settings.wifi.password;

    doc["system"]["hostname"] =
        settings.system.hostname;

    doc["rtc"]["utcOffsetMinutes"] =
        settings.rtc.utcOffsetMinutes;

    doc["rtc"]["timezoneName"] =
        settings.rtc.timezoneName;

    doc["ntp"]["server"] =
        settings.ntp.server;

    doc["ntp"]["lastSync"] =
        settings.ntp.lastSync;

    doc["ntp"]["autoSync"] =
        settings.ntp.autoSync;

    if (
        serializeJson(
            doc,
            file
        ) == 0
    )
    {
        file.close();

        Serial.println("JSON write error");

        return false;
    }

    file.close();

    Serial.println("Configuration saved");

    return true;
}

void StorageManager::reset()
{
    Serial.println("Factory reset");

    LittleFS.remove(
        CONFIG_FILE
    );

    memset(
        &settings,
        0,
        sizeof(settings)
    );

    createDefault();
}

void StorageManager::print()
{
    Serial.println();

    Serial.println(
        "===== SETTINGS ====="
    );

    Serial.print(
        "Version: "
    );

    Serial.println(
        settings.version
    );

    Serial.print(
        "SSID: "
    );

    Serial.println(
        settings.wifi.ssid
    );

    Serial.print(
        "Password: "
    );

    Serial.println(
        settings.wifi.password
    );

    Serial.print(
        "Hostname: "
    );

    Serial.println(
        settings.system.hostname
    );

    Serial.println(
        "===================="
    );

    Serial.println();
}