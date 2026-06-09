#pragma once

#include <Arduino.h>
#include "settings.h"

class StorageManager
{
public:

    bool begin();

    bool load();

    bool save();

    void reset();

};

extern StorageManager storageManager;