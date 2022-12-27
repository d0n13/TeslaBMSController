#include <Arduino.h>
#include "BMSManager.hpp"

#include "display.hpp"

BMSManager packManager = BMSManager();

void setup() {

  Serial.begin(115200);

  initDisplay();

  Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
  Serial.printf("SPIRam Total heap %d, SPIRam Free Heap %d\n", ESP.getPsramSize(), ESP.getFreePsram());
  Serial.printf("ChipRevision %d, Cpu Freq %d, SDK Version %s\n", ESP.getChipRevision(), ESP.getCpuFreqMHz(), ESP.getSdkVersion());
  Serial.printf("Flash Size %d, Flash Speed %d\n", ESP.getFlashChipSize(), ESP.getFlashChipSpeed());
}

void loop() {

    displayText(5, 5, 2, "Tesla BMS", SSD1327_WHITE);
    displayText(5, 22, 1, "Operating Status:", 0x04);

    Serial.println("Get BMS status");

    packManager.findBoards();
    BMSModule module = BMSModule(1);
    module.readStatus();
    module.readModuleValues();
    delay(5000);
}