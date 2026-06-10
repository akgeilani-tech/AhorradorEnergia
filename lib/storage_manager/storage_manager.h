#pragma once

class StorageManager
{
public:

    bool begin();

    bool load();

    bool save();

    bool createDefault();

    void reset();

    void print();
};

extern StorageManager storageManager;