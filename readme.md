# ESP32-IoT-Core

Framework base para proyectos IoT basados en ESP32.

Este proyecto proporciona una infraestructura reutilizable para dispositivos IoT que requieren:

* Configuración WiFi desde navegador
* Almacenamiento persistente
* RTC DS3231
* Sincronización NTP
* Servidor Web integrado
* API REST
* Interfaz Web responsive
* Descubrimiento mediante mDNS

El objetivo es reutilizar esta base en futuros proyectos sin volver a implementar la infraestructura principal.

---

## Características

### Conectividad

* Modo AP (Access Point)
* Modo STA (Station)
* AP de respaldo automático
* Reconexión automática
* Verificación de acceso a Internet
* mDNS (`hostname.local`)

### Gestión de Tiempo

* RTC DS3231
* Sincronización NTP manual
* Sincronización NTP automática
* UTC Offset configurable
* Persistencia de configuración horaria

### Almacenamiento

* LittleFS
* Configuración persistente en JSON
* Restauración de fábrica
* Versionado de configuración

### Web

* Servidor HTTP embebido
* API REST JSON
* Configuración desde navegador
* Interfaz Web responsive

---

## Arquitectura

```text
                 Internet
                     │
                     ▼
                NTP Server
                     │
                     ▼
                NTPManager
                     │
                     ▼
                RTC DS3231
                     │
                     ▼
                RTCManager
                     │
                     ▼
              Fuente Oficial
                 de Tiempo
                     │
      ┌──────────────┼──────────────┐
      ▼              ▼              ▼
 Web Server      Scheduler      Aplicación
```

---

## Estructura del Proyecto

```text
src/
│
├── main.cpp
│
lib/
│
├── config/
│   └── config.h
│
├── settings/
│   ├── settings.h
│   └── settings.cpp
│
├── storage_manager/
│   ├── storage_manager.h
│   └── storage_manager.cpp
│
├── wifi_manager/
│   ├── wifi_manager.h
│   └── wifi_manager.cpp
│
├── rtc_manager/
│   ├── rtc_manager.h
│   └── rtc_manager.cpp
│
├── ntp_manager/
│   ├── ntp_manager.h
│   └── ntp_manager.cpp
│
└── web_server/
    ├── web_server.h
    └── web_server.cpp

data/
│
├── index.html
├── style.css
└── app.js
```

---

## Dependencias

### PlatformIO

```ini
lib_deps =
    adafruit/RTClib
    bblanchon/ArduinoJson
    arduino-libraries/NTPClient
```

### Framework

```ini
framework = arduino
```

### Filesystem

```ini
board_build.filesystem = littlefs
```

---

## Flujo de Arranque

```text
ESP32 Boot
    │
    ▼
LittleFS.begin()
    │
    ▼
StorageManager.load()
    │
    ▼
RTCManager.begin()
    │
    ▼
WifiManager.begin()
    │
    ▼
NTPManager.begin()
    │
    ▼
WebServerManager.begin()
```

---

## Módulos Disponibles

## StorageManager

Responsable de la persistencia de configuración.

### Funciones

```cpp
storageManager.begin();
```

Inicializa LittleFS.

```cpp
storageManager.load();
```

Carga configuración desde archivo JSON.

```cpp
storageManager.save();
```

Guarda configuración actual.

```cpp
storageManager.createDefault();
```

Genera configuración por defecto.

```cpp
storageManager.reset();
```

Restaura configuración de fábrica.

```cpp
storageManager.print();
```

Muestra configuración por consola.

---

## RTCManager

Responsable de la comunicación con DS3231.

### Funciones

```cpp
rtcManager.begin();
```

Inicializa RTC.

```cpp
rtcManager.getDateTime();
```

Obtiene fecha y hora actual.

Retorna:

```cpp
DateTime
```

---

```cpp
rtcManager.getUnixTime();
```

Obtiene timestamp Unix.

Retorna:

```cpp
uint32_t
```

---

```cpp
rtcManager.setDateTime(dt);
```

Establece fecha y hora.

Retorna:

```cpp
bool
```

---

```cpp
rtcManager.isValid();
```

Verifica que el RTC contenga una fecha válida.

Retorna:

```cpp
bool
```

---

## WifiManager

Responsable de toda la conectividad WiFi.

### Funciones

```cpp
wifiManager.begin();
```

Inicialización.

---

```cpp
wifiManager.update();
```

Máquina de estados WiFi.

---

```cpp
wifiManager.requestReconnect();
```

Solicita reconexión.

---

```cpp
wifiManager.isConfigured();
```

Verifica si existe configuración WiFi.

---

```cpp
wifiManager.isConnected();
```

Indica conexión STA activa.

---

```cpp
wifiManager.hasInternet();
```

Verifica acceso a Internet.

---

```cpp
wifiManager.getState();
```

Obtiene estado actual.

---

```cpp
wifiManager.getCurrentSSID();
```

SSID activo.

---

```cpp
wifiManager.getCurrentIP();
```

IP actual.

---

```cpp
wifiManager.getCurrentRSSI();
```

Potencia de señal.

---

```cpp
wifiManager.getCurrentMode();
```

Retorna:

```text
AP
```

o

```text
STA
```

---

## NTPManager

Responsable de la sincronización horaria.

### Funciones

```cpp
ntpManager.begin();
```

Inicialización.

---

```cpp
ntpManager.update();
```

Sincronización automática.

---

```cpp
ntpManager.syncRTC();
```

Sincronización inmediata con servidor NTP.

Retorna:

```cpp
bool
```

---

## WebServerManager

Servidor HTTP integrado.

### Funciones

```cpp
webServerManager.begin();
```

Inicializa servidor.

---

```cpp
webServerManager.update();
```

Procesa peticiones HTTP.

---

## Estructuras de Configuración

### WifiSettings

```cpp
struct WifiSettings
{
    char ssid[32];
    char password[64];
};
```

---

### SystemSettings

```cpp
struct SystemSettings
{
    char hostname[32];
};
```

---

### RTCSettings

```cpp
struct RTCSettings
{
    int16_t utcOffsetMinutes;
    char timezoneName[32];
};
```

---

### NTPSettings

```cpp
struct NTPSettings
{
    char server[64];
    bool autoSync;
    uint32_t lastSync;
};
```

---

## API REST

### Estado del Sistema

```http
GET /api/status
```

---

### Estado RTC

```http
GET /api/rtc
```

Devuelve:

```json
{
  "year": 2026,
  "month": 6,
  "day": 18,
  "hour": 23,
  "minute": 30,
  "second": 51,
  "unix": 1781825451,
  "utcOffset": -180,
  "ntpServer": "pool.ntp.org",
  "autoSync": false,
  "lastSync": 1781825410
}
```

---

### Guardar Configuración RTC

```http
POST /api/rtc
```

---

### Sincronizar RTC

```http
POST /api/rtc/sync
```

---

### Obtener Configuración WiFi

```http
GET /api/wifi
```

---

### Guardar Configuración WiFi

```http
POST /api/wifi
```

---

### Reiniciar Dispositivo

```http
POST /api/restart
```

---

### Restaurar Fábrica

```http
POST /api/factory-reset
```

---

## Máquina de Estados WiFi

Estados implementados:

```cpp
WIFI_IDLE
WIFI_AP_ONLY
WIFI_CONNECTING
WIFI_CONNECTED
```

---

## mDNS

Cuando existe conexión STA válida:

```text
http://hostname.local
```

Ejemplo:

```text
http://ahorrador.local
```

---

## Casos de Uso

Este núcleo puede utilizarse como base para:

* Automatización residencial
* Control de iluminación
* Sistemas de riego
* Acuarios
* Calentadores de agua
* Bombas de agua
* Piscinas
* Monitoreo ambiental
* Energía solar
* Control de tanques
* Temporizadores programables

---

## Buenas Prácticas

### Tiempo

Utilizar siempre:

```cpp
rtcManager.getDateTime();
```

como fuente oficial de tiempo.

### Persistencia

Después de modificar configuraciones:

```cpp
storageManager.save();
```

### NTP

Utilizar exclusivamente:

```cpp
ntpManager.syncRTC();
```

para sincronización.

### WiFi

No acceder directamente a la clase WiFi desde la lógica de negocio.

Utilizar:

```cpp
wifiManager
```

como capa de abstracción.

---

## Roadmap

### Próximas mejoras

* Scheduler genérico
* MQTT
* OTA Update
* HTTPS
* Logs persistentes
* Sistema de eventos
* Soporte para múltiples relés
* Integración Home Assistant

---

## Licencia

MIT License
