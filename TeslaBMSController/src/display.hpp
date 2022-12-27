#include <Arduino.h>
#include <Adafruit_SSD1327.h> 
#include <Adafruit_GFX.h>

void updateDisplay();
void initDisplay();
void setCursor(int x, int y);
void displayText(int x, int y, int size, const String &text, int colour);
void displayLineText(const String &text);