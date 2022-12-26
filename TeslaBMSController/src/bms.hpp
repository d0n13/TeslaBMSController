#include <Arduino.h>
#include "comms.hpp"

#define REG_DEV_STATUS      1
#define REG_GPAI            1
#define REG_VCELL1          3
#define REG_VCELL2          5
#define REG_VCELL3          7
#define REG_VCELL4          9
#define REG_VCELL5          0xB
#define REG_VCELL6          0xD
#define REG_TEMPERATURE1    0xF
#define REG_TEMPERATURE2    0x11
#define REG_ALERT_STATUS    0x20
#define REG_FAULT_STATUS    0x21
#define REG_COV_FAULT       0x22
#define REG_CUV_FAULT       0x23
#define REG_ADC_CTRL        0x30
#define REG_IO_CTRL         0x31
#define REG_BAL_CTRL        0x32
#define REG_BAL_TIME        0x33
#define REG_ADC_CONV        0x34
#define REG_ADDR_CTRL       0x3B

#define MAX_MODULE_ADDR 0x3E

#define EEPROM_VERSION      0x10    //update any time EEPROM struct below is changed.
#define EEPROM_PAGE         0


class BMSModule {

    private:

        Comms comms;

        float cellVolt[6];          // calculated as 16 bit value * 6.250 / 16383 = volts
        float lowestCellVolt[6];
        float highestCellVolt[6];
        float moduleVolt;          // calculated as 16 bit value * 33.333 / 16383 = volts
        float temperatures[2];     // Don't know the proper scaling at this point
        float lowestTemperature;
        float highestTemperature;
        float lowestModuleVolt;
        float highestModuleVolt;
        float IgnoreCell;
        bool exists;
        int alerts;
        int faults;
        int COVFaults;
        int CUVFaults;
        int sensor;
        uint8_t moduleAddress;     //1 to 0x3E

    public:

        BMSModule(uint8_t address);
        ~BMSModule();
        void readStatus();
        bool readModuleValues();

        void setExists(bool ex);
        void setAddress(int newAddr);

        float getLowTemp();
        float getHighTemp();
};