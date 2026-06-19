# ESP32-IoT-Core

Framework base para proyectos IoT basados en ESP32, diseñado para proporcionar una infraestructura sólida, reutilizable y desacoplada de la lógica de negocio.

El objetivo es evitar reimplementar en cada proyecto los mismos componentes fundamentales:

- WiFi AP/STA
- RTC
- NTP
- Almacenamiento persistente
- Servidor Web
- API REST
- Gestión de configuración
- Sincronización horaria
- Interfaz Web

---

## Características

### Conectividad

- Modo STA (cliente WiFi)
- Modo AP (portal de configuración)
- Reconexión automática
- Verificación de acceso a Internet
- Estado de conexión centralizado

### Gestión de Tiempo

- RTC DS3231
- Sincronización NTP manual
- Sincronización NTP automática
- Soporte UTC Offset
- Fuente única de tiempo para toda la aplicación

### Persistencia

- LittleFS
- Configuración persistente
- Restauración automática al arranque
- Valores por defecto configurables

### Web

- Servidor HTTP integrado
- API REST JSON
- Interfaz Web responsive
- Configuración desde navegador

### Arquitectura

- Modular
- Orientada a objetos
- Bajo acoplamiento
- Reutilizable en múltiples proyectos

---

## Arquitectura General

```text
                    Internet
                        │
                        ▼
                  NTP Server
                        │
                        ▼
                  NTPManager
                        │
                Aplica UTC Offset
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
        ┌───────────────┼───────────────┐
        ▼               ▼               ▼
   Scheduler      Web Server      Aplicación
```

---

## Principios de Diseño

### Regla 1

El RTC almacena siempre hora local.

Ejemplo:

UTC obtenido desde NTP:

```text
18:00
```

UTC Offset:

```text
-4 horas
```

Hora almacenada en RTC:

```text
14:00
```

---

### Regla 2

NTP solamente sincroniza el RTC.

Ningún otro módulo debe consultar directamente servidores NTP.

---

### Regla 3

RTCManager es la única fuente oficial de tiempo.

Uso permitido:

```cpp
DateTime now =
    rtcManager.now();
```

Uso no permitido:

```cpp
time(nullptr);

NTPClient.getEpochTime();

configTime(...);
```

---

### Regla 4

Todo el sistema debe trabajar con la hora obtenida desde RTCManager.

Ejemplos:

- Scheduler
- Historial
- Relés
- Automatización
- Interfaz Web

---

### Regla 5

El UTC Offset se aplica una sola vez.

```text
NTP UTC
    │
    ▼
Aplicar Offset
    │
    ▼
RTC
```

Nunca volver a aplicar el offset posteriormente.

---

## Estructura del Proyecto

```text
src/
│
├── core/
│   ├── rtc_manager.*
│   ├── ntp_manager.*
│   ├── wifi_manager.*
│   ├── storage_manager.*
│   └── web_server_manager.*
│
├── config/
│   ├── settings.h
│   └── defaults.h
│
├── app/
│   └── application.cpp
│
└── main.cpp
```

---

## Componentes

### RTCManager

Responsable de:

- Inicializar RTC
- Leer fecha y hora
- Escribir fecha y hora
- Validar RTC

Funciones principales:

```cpp
rtcManager.begin();

rtcManager.now();

rtcManager.setDateTime(dt);
```

---

### NTPManager

Responsable de:

- Sincronización NTP
- Aplicación de UTC Offset
- Actualización del RTC

Funciones principales:

```cpp
ntpManager.syncRTC();
```

---

### WiFiManager

Responsable de:

- Conexión STA
- Modo AP
- Detección de Internet
- Estado de red

Funciones principales:

```cpp
wifiManager.begin();

wifiManager.isConnected();

wifiManager.hasInternet();
```

---

### StorageManager

Responsable de:

- Guardar configuración
- Cargar configuración
- Valores por defecto

Funciones principales:

```cpp
storageManager.load();

storageManager.save();

storageManager.reset();
```

---

### WebServerManager

Responsable de:

- API REST
- Servidor Web
- Configuración remota

Funciones principales:

```cpp
webServerManager.begin();

webServerManager.handleClient();
```

---

## Configuración Persistente

### Estructuras de Configuración

#### RTCSettings

```cpp
struct RTCSettings
{
    int utcOffsetMinutes;
};
```

---

#### NTPSettings

```cpp
struct NTPSettings
{
    char server[64];

    bool autoSync;

    uint32_t lastSync;
};
```

---

### WiFiSettings

```cpp
struct WiFiSettings
{
    char ssid[32];

    char password[64];
};
```

---

## API REST

### Obtener Estado RTC

#### GET

```http
/api/rtc
```

Respuesta:

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

#### POST /api/rtc

```http
/api/rtc
```

Body:

```json
{
  "year": 2026,
  "month": 6,
  "day": 18,
  "hour": 23,
  "minute": 30,
  "second": 0,
  "utcOffset": -180,
  "ntpServer": "pool.ntp.org",
  "autoSync": true
}
```

---

### Sincronizar RTC

#### POST /api/rtc/sync

```http
/api/rtc/sync
```

Body:

```json
{
  "utcOffset": -180,
  "ntpServer": "pool.ntp.org",
  "autoSync": true
}
```

Respuesta:

```text
NTP Sync OK
```

---

## Flujo de Arranque

```text
Boot
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
WiFiManager.begin()
 │
 ▼
WebServerManager.begin()
 │
 ▼
Sistema Operativo
```

---

## Flujo de Sincronización

```text
Usuario
    │
    ▼
/api/rtc/sync
    │
    ▼
Guardar Configuración
    │
    ▼
Consultar NTP
    │
    ▼
Aplicar UTC Offset
    │
    ▼
Actualizar RTC
    │
    ▼
Guardar lastSync
```

---

## Casos de Uso

Este framework puede utilizarse como base para:

- Calentadores de agua
- Acuarios
- Sistemas de riego
- Automatización residencial
- Control de iluminación
- Bombas de agua
- Piscinas
- Tanques de almacenamiento
- Monitoreo ambiental
- Sistemas de energía solar

---

## Recomendaciones

### Fuente de tiempo

Utilizar siempre:

```cpp
rtcManager.now();
```

---

### Almacenamiento

Después de modificar configuraciones:

```cpp
storageManager.save();
```

---

### Sincronización

No utilizar NTP directamente fuera de NTPManager.

---

### Escalabilidad

Mantener la lógica específica de cada proyecto fuera de:

- RTCManager
- NTPManager
- WiFiManager
- StorageManager
- WebServerManager

Estos módulos deben permanecer genéricos y reutilizables.

---

## Estado Actual

Versión:

```text
v1.0.0
```

Incluye:

- RTC DS3231
- NTP
- UTC Offset
- WiFi AP/STA
- LittleFS
- Servidor Web
- API REST
- Configuración Persistente
- Interfaz Web Responsive

Base estable para futuros proyectos IoT.
