#include <Arduino.h>
#include "bms.hpp"
#include "comms.hpp"

class BMSManager {

    private:

        // BMSModule modules[MAX_MODULE_ADDR + 1];
        int numFoundModules;

    public:

        BMSManager();
        ~BMSManager();
        void findBoards();
};