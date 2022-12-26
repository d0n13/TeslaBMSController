#include "BMSManager.hpp"

BMSManager::BMSManager() {

    TeslaBMSSerial.begin(TeslaBMSBaudRate);

    // for (int i = 1; i <= MAX_MODULE_ADDR; i++) {
    //     modules[i].setExists(false);
    //     modules[i].setAddress(i);
    // }
}

BMSManager::~BMSManager() {

    TeslaBMSSerial.end();
}

void BMSManager::findBoards()
{
    Comms comms;
    uint8_t payload[3];
    uint8_t buff[8];

    numFoundModules = 0;
    payload[0] = 0;
    payload[1] = 0; //read registers starting at 0
    payload[2] = 1; //read one byte
    int x = 1;
    // for (int x = 1; x <= 5; x++)
    // {
        // modules[x].setExists(false);
        payload[0] = x << 1;
        comms.sendData(payload, 3, false);
        delay(10);
        if (comms.getReply(buff, 8) > 4)
        {
            if (buff[0] == (x << 1) && buff[1] == 0 && buff[2] == 1 && buff[4] > 0) {
                // modules[x].setExists(true);

                numFoundModules++;
                Serial.printf("Found module with address: %X\n", x); 
            }
        }
        delay(1);
    // }
}