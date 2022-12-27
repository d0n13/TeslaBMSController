#pragma once

#include <Arduino.h>
#include <Adafruit_SSD1327.h> 
#include <Adafruit_GFX.h>
#include "BMSManager.hpp"
#include <Fonts/FreeSansBold24pt7b.h>

void showPackStatusScreen(BMSManager &manager);
void showVoltageScreen(BMSManager &manager);


void clearScreen();
void updateDisplay();
void initDisplay();
void setCursor(int x, int y);
void displayText(int x, int y, int size, const String &text, int colour);
void displayLineText(const String &text);

void displayPackVoltages(BMSManager &manager);
void displayPackTempatures(BMSManager &manager);

void displayPackMainVoltage(BMSManager &manager);