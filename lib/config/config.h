#pragma once

#define SDA_PIN 8
#define SCL_PIN 9

#define AP_SSID "EnergySetup"
#define AP_PASSWORD "12345678"

#define HOSTNAME "energy-saver"

#define TIMEZONE_OFFSET 0 //-240
#define NTP_SERVER "pool.ntp.org"
#define SYNC_INTERVAL 86400000UL // 24h

#define CONFIG_FILE "/config.json"
#define SETTINGS_MAGIC 1
#define SETTINGS_VERSION 1

#define WIFI_CONNECT_TIMEOUT 15000