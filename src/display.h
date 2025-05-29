#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include "globals.h"

// Display pins
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_SDA     2
#define OLED_SCL     3

// Display dimensions
#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 64

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32



// Initialize display
void initDisplay();

// Clear display
void clearDisplay();

// Update display with current readings
void updateDisplay();

// Display a "no data" message
void displayNoData();

// Display a splash screen
void displaySplashScreen();


extern Adafruit_SSD1306 display;

#endif // DISPLAY_H
