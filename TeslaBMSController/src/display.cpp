#include "display.hpp"

Adafruit_SSD1327 display = Adafruit_SSD1327(128, 128);

void initDisplay() {

    Wire.begin (21, 22);   // sda= GPIO_21 /scl= GPIO_22
    display.begin(0x3D);
    
    if ( ! display.begin(0x3D) ) {
        Serial.println("Unable to initialize OLED");
        while (1) yield();
    }
}

void setCursor(int x, int y) {

    display.setCursor(x, y);
}
void displayText(int x, int y, int size, const String &text, int colour) {

    display.setCursor(x, y);
    display.setTextSize(size);
    display.setTextColor(colour);
    display.print(text);
    updateDisplay();
}

void displayLineText(const String &text) {

    display.println(text);
    updateDisplay();
}

void updateDisplay() {

    display.display();
}