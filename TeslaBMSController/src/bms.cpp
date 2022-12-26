#include "bms.hpp"

BMSModule::BMSModule(uint8_t address) {

    moduleAddress = address;
}

BMSModule::~BMSModule() { 
}

void BMSModule::readStatus() {

    uint8_t payload[3];
    uint8_t buff[8];
    payload[0] = moduleAddress << 1;    //adresss
    payload[1] = REG_ALERT_STATUS;      //Alert Status start
    payload[2] = 0x04;
    comms.sendDataWithReply(payload, 3, false, buff, 7);
    alerts = buff[3];
    faults = buff[4];
    COVFaults = buff[5];
    CUVFaults = buff[6];
}

bool BMSModule::readModuleValues()
{
    uint8_t payload[4];
    uint8_t buff[50];
    uint8_t calcCRC;
    bool retVal = false;
    int retLen;
    float tempCalc;
    float tempTemp;
    
    payload[0] = moduleAddress << 1;
    
    readStatus();
    Serial.printf("Module %i   alerts=%X   faults=%X   COV=%X   CUV=%X\n", moduleAddress, alerts, faults, COVFaults, CUVFaults);
    
    payload[1] = REG_ADC_CTRL;
    payload[2] = 0b00111101; //ADC Auto mode, read every ADC input we can (Both Temps, Pack, 6 cells)
    comms.sendDataWithReply(payload, 3, true, buff, 3);
 
    payload[1] = REG_IO_CTRL;
    payload[2] = 0b00000011; //enable temperature measurement VSS pins
    comms.sendDataWithReply(payload, 3, true, buff, 3);
            
    payload[1] = REG_ADC_CONV; //start all ADC conversions
    payload[2] = 1;
    comms.sendDataWithReply(payload, 3, true, buff, 3);
                
    payload[1] = REG_GPAI; //start reading registers at the module voltage registers
    payload[2] = 0x12; //read 18 bytes (Each value takes 2 - ModuleV, CellV1-6, Temp1, Temp2)
    retLen = comms.sendDataWithReply(payload, 3, false, buff, 22);
            
    calcCRC = comms.crc(buff, retLen-1);
    // Serial.printf("Sent CRC: %x     Calculated CRC: %x", buff[21], calcCRC);
    
    
    //18 data bytes, address, command, length, and CRC = 22 bytes returned
    //Also validate CRC to ensure we didn't get garbage data.
    if ( (retLen == 22) && (buff[21] == calcCRC) )
    {
        if (buff[0] == (moduleAddress << 1) && buff[1] == REG_GPAI && buff[2] == 0x12) //Also ensure this is actually the reply to our intended query
        {
            //payload is 2 bytes gpai, 2 bytes for each of 6 cell voltages, 2 bytes for each of two temperatures (18 bytes of data)
            moduleVolt = (buff[3] * 256 + buff[4]) * 0.002034609f;
            if (moduleVolt > highestModuleVolt) highestModuleVolt = moduleVolt;
            if (moduleVolt < lowestModuleVolt) lowestModuleVolt = moduleVolt;            
            for (int i = 0; i < 6; i++) 
            {
                cellVolt[i] = (buff[5 + (i * 2)] * 256 + buff[6 + (i * 2)]) * 0.000381493f;
                if (lowestCellVolt[i] > cellVolt[i] && cellVolt[i] >= IgnoreCell) lowestCellVolt[i] = cellVolt[i];
                if (highestCellVolt[i] < cellVolt[i]) highestCellVolt[i] = cellVolt[i];
            }
            
            //Now using steinhart/hart equation for temperatures. We'll see if it is better than old code.
            tempTemp = (1.78f / ((buff[17] * 256 + buff[18] + 2) / 33046.0f) - 3.57f);
            tempTemp *= 1000.0f;
            tempCalc =  1.0f / (0.0007610373573f + (0.0002728524832 * logf(tempTemp)) + (powf(logf(tempTemp), 3) * 0.0000001022822735f));            
            
            temperatures[0] = tempCalc - 273.15f;            
            
            tempTemp = 1.78f / ((buff[19] * 256 + buff[20] + 9) / 33068.0f) - 3.57f;
            tempTemp *= 1000.0f;
            tempCalc = 1.0f / (0.0007610373573f + (0.0002728524832 * logf(tempTemp)) + (powf(logf(tempTemp), 3) * 0.0000001022822735f));
            temperatures[1] = tempCalc - 273.15f;
            
            if (getLowTemp() < lowestTemperature) lowestTemperature = getLowTemp();
            if (getHighTemp() > highestTemperature) highestTemperature = getHighTemp();

            Serial.print("Got voltage and temperature readings");
            retVal = true;

            Serial.printf("Temp 0: %f\n", temperatures[0]);
            Serial.printf("Temp 1: %f\n", temperatures[1]);

            for (int i = 0; i < 6; i++) 
            {
                Serial.printf("Cell %i: %0.2f\n", i, cellVolt[i]);
            }
        }        
    }
    else
    {
        Serial.printf("Invalid module response received for module %i  len: %i   crc: %i   calc: %i\n", 
                      moduleAddress, retLen, buff[21], calcCRC);
    }
     
     //turning the temperature wires off here seems to cause weird temperature glitches
   // payload[1] = REG_IO_CTRL;
   // payload[2] = 0b00000000; //turn off temperature measurement pins
   // BMSUtil::sendData(payload, 3, true);
   // delay(3);        
   // BMSUtil::getReply(buff, 50);    //TODO: we're not validating the reply here. Perhaps check to see if a valid reply came back    
    
    return retVal;
}

void BMSModule::setExists(bool ex)
{
    exists = ex;
}

void BMSModule::setAddress(int newAddr)
{
    if (newAddr < 0 || newAddr > MAX_MODULE_ADDR) 
        return;
    moduleAddress = newAddr;
}

float BMSModule::getLowTemp()
{
   return (temperatures[0] < temperatures[1]) ? temperatures[0] : temperatures[1]; 
}

float BMSModule::getHighTemp()
{
   return (temperatures[0] < temperatures[1]) ? temperatures[1] : temperatures[0];     
}