#include <Arduino.h>
#include "driver/gpio.h"

#define LED GPIO_NUM_1
#define Console Serial
#define TeslaBMS Serial2
#define TeslaBMSBaudRate 615200

void setup() {

  Console.begin(9600);
  TeslaBMS.begin(TeslaBMSBaudRate);

  gpio_set_direction(LED, GPIO_MODE_OUTPUT); 

  Console.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
  Console.printf("SPIRam Total heap %d, SPIRam Free Heap %d\n", ESP.getPsramSize(), ESP.getFreePsram());
  Console.printf("ChipRevision %d, Cpu Freq %d, SDK Version %s\n", ESP.getChipRevision(), ESP.getCpuFreqMHz(), ESP.getSdkVersion());
  Console.printf("Flash Size %d, Flash Speed %d\n", ESP.getFlashChipSize(), ESP.getFlashChipSpeed());
}

void loop() {
  
  digitalWrite(LED, HIGH);
  Console.println("LED is on");
  delay(1000);
  digitalWrite(LED, LOW);
  Console.println("LED is off");
  delay(1000);
}