#include "display-routines.h"
#include "display.h"
#include "globals.h"

#include <Fonts/FreeSerif18pt7b.h>


void drawTaskbarText(Adafruit_SSD1306& lcd, const char* text) {

    // Set text properties
    lcd.setTextSize(2);
    lcd.setTextColor(SSD1306_BLACK);


    // Calculate width of text (each character is approximately 12 pixels wide in size 2)
    int textWidth = strlen(text) * 12;

    // Calculate starting X position to center text
    int x = (DISPLAY_WIDTH - textWidth) / 2;
    if (x < 0) x = 0; // Ensure x doesn't go negative if text is too long

    // Position at bottom of display (64 - 16 = 48)
    int y = DISPLAY_HEIGHT - 16;


    // Draw solid box behind text
    lcd.fillRect(0, y, DISPLAY_WIDTH, 16, SSD1306_WHITE);

    // Set cursor and print text
    lcd.setCursor(x, y + 1);
    lcd.print(text);

    // Reset
    lcd.setTextColor(SSD1306_WHITE);

}

// You can add more display routine functions here
void drawProgressBar(Adafruit_SSD1306 &lcd, int x, int y, int width, int height, int percentage) {
    // Constrain percentage to 0-100 range
    percentage = constrain(percentage, 0, 100);
    
    // Draw the outline of the progress bar
    lcd.drawRect(x, y, width, height, SSD1306_WHITE);
    
    // Calculate the width of the filled portion
    int fillWidth = (width - 2) * percentage / 100;
    
    // Fill the progress bar
    lcd.fillRect(x + 1, y + 1, fillWidth, height - 2, SSD1306_WHITE);
}

void drawValueWithLabel(Adafruit_SSD1306 &lcd, int x, int y, const char* label, int value, const char* units) {
    lcd.setTextColor(SSD1306_WHITE);
    
    // Draw the label
    lcd.setTextSize(1);
    lcd.setCursor(x, y);
    lcd.print(label);
    
    // Draw the value
    lcd.setTextSize(2);
    lcd.setCursor(x + 30, y);
    lcd.print(value);
    
    // Draw units if provided
    if (units && strlen(units) > 0) {
        lcd.setTextSize(1);
        int valueWidth = 0;
        
        // Calculate value width based on number of digits
        if (value < 0) {
            // Negative value
            int absValue = abs(value);
            if (absValue < 10) valueWidth = 18; // -d
            else if (absValue < 100) valueWidth = 24; // -dd
            else if (absValue < 1000) valueWidth = 30; // -ddd
            else valueWidth = 36; // -dddd
        } else {
            // Positive value
            if (value < 10) valueWidth = 12; // d
            else if (value < 100) valueWidth = 18; // dd
            else if (value < 1000) valueWidth = 24; // ddd
            else valueWidth = 30; // dddd
        }
        
        lcd.setCursor(x + 30 + valueWidth, y + 5);
        lcd.print(units);
    }
}

void drawHorizontalGauge(Adafruit_SSD1306 &lcd, int x, int y, int width, int height, int value, int minValue, int maxValue) {
    // Constrain value to min-max range
    value = constrain(value, minValue, maxValue);
    
    // Draw the outline of the gauge
    lcd.drawRect(x, y, width, height, SSD1306_WHITE);
    
    // Calculate the width of the filled portion
    int fillWidth = map(value, minValue, maxValue, 0, width - 2);
    
    // Fill the gauge
    lcd.fillRect(x + 1, y + 1, fillWidth, height - 2, SSD1306_WHITE);
    
    // Draw min/max values
    lcd.setTextSize(1);
    lcd.setCursor(x, y + height + 1);
    lcd.print(minValue);
    
    // Right-align max value
    char buffer[8];
    sprintf(buffer, "%d", maxValue);
    int maxValueWidth = strlen(buffer) * 6; // Approximate width in pixels
    lcd.setCursor(x + width - maxValueWidth, y + height + 1);
    lcd.print(maxValue);
}

void drawVerticalGauge(Adafruit_SSD1306 &lcd, int x, int y, int width, int height, int value, int minValue, int maxValue) {
    // Constrain value to min-max range
    value = constrain(value, minValue, maxValue);
    
    // Draw the outline of the gauge
    lcd.drawRect(x, y, width, height, SSD1306_WHITE);
    
    // Calculate the height of the filled portion (starting from bottom)
    int fillHeight = map(value, minValue, maxValue, 0, height - 2);
    
    // Fill the gauge from bottom up
    lcd.fillRect(x + 1, y + height - 1 - fillHeight, width - 2, fillHeight, SSD1306_WHITE);
    
    // Draw min/max values
    lcd.setTextSize(1);
    lcd.setCursor(x + width + 2, y + height - 8);
    lcd.print(minValue);
    
    lcd.setCursor(x + width + 2, y);
    lcd.print(maxValue);
}
