#pragma once

#include <Arduino.h>

#define TeslaBMSSerial Serial2
#define TeslaBMSBaudRate 615200

class Comms {

public:
    uint8_t crc(uint8_t *input, int lenInput);
    void sendData(uint8_t *data, uint8_t dataLen, bool isWrite);
    int getReply(uint8_t *data, int maxLen);
    int sendDataWithReply(uint8_t *data, uint8_t dataLen, bool isWrite, uint8_t *retData, int retLen);
};