#include "comms.hpp"

uint8_t Comms::crc(uint8_t *input, int lenInput) {

        uint8_t generator = 0x07;
        uint8_t crc = 0;

        for (int x = 0; x < lenInput; x++)
        {
            crc ^= input[x]; /* XOR-in the next input byte */

            for (int i = 0; i < 8; i++)
            {
                if ((crc & 0x80) != 0)
                {
                    crc = (uint8_t)((crc << 1) ^ generator);
                }
                else
                {
                    crc <<= 1;
                }
            }
        }

        return crc;
    }

void Comms::sendData(uint8_t *data, uint8_t dataLen, bool isWrite)
{
    uint8_t orig = data[0];
    uint8_t addrByte = data[0];
    if (isWrite) addrByte |= 1;
    TeslaBMSSerial.write(addrByte);
    TeslaBMSSerial.write(&data[1], dataLen - 1);  // At least 2 bytes sent every time. There should be, addr and cmd at the least.
    data[0] = addrByte;
    if (isWrite)
        TeslaBMSSerial.write(crc(data, dataLen));        

    // Serial.print("\nWriting\n");
    // Serial.print(addrByte, HEX);
    // Serial.print(" ");
    
    // for (int x = 1; x < dataLen; x++) {
    //     Serial.print(data[x], HEX);
    //     Serial.print(" ");
    // }
    
    // if (isWrite) 
    //     Serial.print(crc(data, dataLen), HEX);
    // Serial.print("\n");
    data[0] = orig; // Reset address back because we added the write bit
}

int Comms::getReply(uint8_t *data, int maxLen)
{ 
    int numBytes = 0; 
    // Serial.print("\nReading\n");
    while (TeslaBMSSerial.available() && numBytes < maxLen)
    {
        data[numBytes] = TeslaBMSSerial.read();
        
        // Serial.print(data[numBytes], HEX);
        // Serial.print(" ");
    
        numBytes++;
    }
    // Serial.print("\n");
    if (maxLen == numBytes)
    {
        while (TeslaBMSSerial.available()) 
            TeslaBMSSerial.read();
    }
    // Serial.printf("\nBytes recieved: %d\n", numBytes);
    return numBytes;
}

//Uses above functions to send data then get the response. Will auto retry if response not 
//the expected return length. This helps to alleviate any comm issues. The Due cannot exactly
//match the correct comm speed so sometimes there are data glitches.
int Comms::sendDataWithReply(uint8_t *data, uint8_t dataLen, bool isWrite, uint8_t *retData, int retLen)
{
    int attempts = 1;
    int returnedLength;
    while (attempts < 4)
    {
        sendData(data, dataLen, isWrite);
        delay(2 * ((retLen / 8) + 1));
        returnedLength = getReply(retData, retLen);
        // Serial.printf("Send %d Got %d", dataLen, returnedLength);
        if (returnedLength == retLen) return returnedLength;
        attempts++;
    }
    return returnedLength; //failed to get a proper response.
}