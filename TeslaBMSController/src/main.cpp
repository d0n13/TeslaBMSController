#include <Arduino.h>
#include "BMSManager.hpp"
#include "display.hpp"

#define SCREENTIME 6000

BMSManager packManager = BMSManager();

void setup() {

  Serial.begin(115200);

  initDisplay();
  packManager.findBoards();

  // Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
  // Serial.printf("SPIRam Total heap %d, SPIRam Free Heap %d\n", ESP.getPsramSize(), ESP.getFreePsram());
  // Serial.printf("ChipRevision %d, Cpu Freq %d, SDK Version %s\n", ESP.getChipRevision(), ESP.getCpuFreqMHz(), ESP.getSdkVersion());
  // Serial.printf("Flash Size %d, Flash Speed %d\n", ESP.getFlashChipSize(), ESP.getFlashChipSpeed());
}

void loop() {

    while(true) {

      packManager.modules[0].readModule();

      showPackStatusScreen(packManager);

      delay(SCREENTIME);

      showVoltageScreen(packManager);

      delay(SCREENTIME);
    }
}
