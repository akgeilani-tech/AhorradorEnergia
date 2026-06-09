#pragma once

class NTPManager {

public:

    void begin();

    bool syncRTC();
    
    void update();
};