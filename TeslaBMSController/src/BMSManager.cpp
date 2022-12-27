#include "BMSManager.hpp"

BMSManager::BMSManager() {

    Serial.print("Starting serial");
    TeslaBMSSerial.begin(TeslaBMSBaudRate);
}

BMSManager::~BMSManager() {

    Serial.print("Ending serial");
    TeslaBMSSerial.end();
}

void BMSManager::findBoards()
{
    Comms comms;
    uint8_t payload[3];
    uint8_t buff[8];

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
                
                modules[0].setExists(true);
                modules[0].setAddress(x);

                Serial.printf("Found module with address: %X\n", x); 
            }
        }
        delay(1);
    // }
}

float BMSManager::getPackVoltage() {

    return modules[0].getPackVoltage();
}

