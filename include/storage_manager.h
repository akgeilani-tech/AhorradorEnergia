#pragma once

#include "settings.h"

class StorageManager {

public:

    void begin();

    void save(Settings &s);

    void load(Settings &s);

    void reset();
};