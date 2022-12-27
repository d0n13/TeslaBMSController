#pragma once

#include <Arduino.h>
#include "bms.hpp"
#include "comms.hpp"

class BMSManager {
    private:
        

    public:
        BMSManager();
        ~BMSManager();
        void findBoards();
        float getPackVoltage();

        BMSModule modules[1];
};