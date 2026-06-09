#include "storage_manager.h"

#include <LittleFS.h>
#include <ArduinoJson.h>

#include "config.h"

StorageManager storageManager;

bool StorageManager::begin()
{
    return LittleFS.begin(true);
}

bool StorageManager::load()
{
    // Leer config.json

    return true;
}

bool StorageManager::save()
{
    // Guardar config.json

    return true;
}

void StorageManager::reset()
{
    LittleFS.remove(CONFIG_FILE);
}