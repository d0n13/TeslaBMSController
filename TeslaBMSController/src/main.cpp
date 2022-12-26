#include <Arduino.h>
#include "driver/gpio.h"
#include "BMSManager.hpp"

void setup() {

  Serial.begin(9600);

  Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
  Serial.printf("SPIRam Total heap %d, SPIRam Free Heap %d\n", ESP.getPsramSize(), ESP.getFreePsram());
  Serial.printf("ChipRevision %d, Cpu Freq %d, SDK Version %s\n", ESP.getChipRevision(), ESP.getCpuFreqMHz(), ESP.getSdkVersion());
  Serial.printf("Flash Size %d, Flash Speed %d\n", ESP.getFlashChipSize(), ESP.getFlashChipSpeed());
}

void loop() {
  
    // Initialise BMS Module
    BMSManager packManager = BMSManager();
    
    
    Serial.println("Get BMS status");

    packManager.findBoards();
    BMSModule module = BMSModule(1);
    module.readStatus();
    module.readModuleValues();
    delay(5000);
}