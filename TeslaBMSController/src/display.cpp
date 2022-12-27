#include "display.hpp"
#include "BMSManager.hpp"

Adafruit_SSD1327 display = Adafruit_SSD1327(128, 128, &Wire, -1, 1000000UL);

void initDisplay() {

    Wire.begin (21, 22, 1000000UL);   // sda= GPIO_21 /scl= GPIO_22
    display.begin(0x3D);
    display.fillScreen(0);
    
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
}

void displayLineText(const String &text) {

    display.println(text);
}

void clearScreen() {

    display.fillScreen(0);
}
void updateDisplay() {

    display.display();
}

void displayPackVoltages(BMSManager &manager) {

    for (int i = 0; i < 6; i++) 
    {
        float barWidth = 100;
        char cell[20];
        float voltage = manager.modules[0].getCellVoltage(i);

        sprintf(cell, "%i", i+1);
        displayText(3, 35 + (i * 10), 1, cell, 0xA);
        display.drawRect(13, 35 + (i * 10), barWidth + 2, 8, 0x4);

        float soc = (voltage - 3.0) * 83.333333;
        display.fillRect(14, 36 + (i * 10), soc, 6, 0xF);
    }
}

void displayPackTempatures(BMSManager &manager) {

    char temp1[20];
    char temp2[20];
    sprintf(temp1, "Temp1: %0.2f", manager.modules[0].getTemperature(0));
    sprintf(temp2, "Temp2: %0.2f", manager.modules[0].getTemperature(1));   
    // Serial.printf(temp1);
    // Serial.printf(temp2);
    displayText(3, 98, 1, temp1, 0xA);
    displayText(3, 108, 1, temp2, 0xA);
}

void displayPackMainVoltage(BMSManager &manager) {

    char voltage[10];
    sprintf(voltage, "%0.1fv", manager.modules[0].getPackVoltage());
    displayText(90, 108, 1, voltage, 0xF);
}

void showPackStatusScreen(BMSManager &manager) {

    clearScreen();
    display.setFont();
    displayText(5, 5, 2, "Tesla BMS", SSD1327_WHITE);
    displayText(5, 22, 1, "Operating Status:", 0x04);

    displayPackVoltages(manager);
    displayPackMainVoltage(manager);
    displayPackTempatures(manager);
    updateDisplay();
}

void drawPackSOC(BMSManager &manager) {

    float graphWidth = 100;
    float minVoltage = 18.0;
    float maxVoltage = 25.2;
    float packVoltage = manager.modules[0].getPackVoltage();

    float multiplier =  100 / (maxVoltage - minVoltage);
    float soc = (packVoltage - minVoltage) * multiplier;
    Serial.printf("mult: %.2f", multiplier);
    Serial.printf("soc: %.2f", soc);
    display.drawRect(12, 104, graphWidth + 4, 14, 0xF);
    // Draw inside the rect above
    display.fillRect(14, 106, soc, 10, 0xF);
}

void showVoltageScreen(BMSManager &manager) {

    int16_t x1, y1;
    uint16_t w, h;

    clearScreen();

    display.setFont(&FreeSansBold24pt7b);
    char voltage[10];
    sprintf(voltage, "%0.1fv", manager.modules[0].getPackVoltage());
    display.getTextBounds(voltage, 50, 50, &x1, &y1, &w, &h);
    int x = (display.width() - w) / 2;
    int y = (display.height() - h);
    displayText(x, display.height() - y, 1, voltage, 0xF);
    display.setFont();

    drawPackSOC(manager);
    updateDisplay();
}

