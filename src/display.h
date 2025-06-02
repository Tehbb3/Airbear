#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include "globals.h"

// Display pins
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_SDA     6
#define OLED_SCL     7

// Display dimensions
#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 64

#define SCREEN_1_ADDRESS 0x3C
#define SCREEN_2_ADDRESS 0x3D

// Screen types
#define SCREEN_MAIN            0
#define SCREEN_GRAPH           1
#define SCREEN_STATUS          2
#define SCREEN_DIAGNOSTIC      3
#define SCREEN_DIAGNOSTIC_2    4
#define SCREEN_ENGINE          5
#define SCREEN_TEMPS           6
#define SCREEN_FUELING         7
#define SCREEN_LOADING         8
#define SCREEN_NOTIFICATION    9
#define SCREEN_NO_ECU_DATA     10
#define SCREEN_SPLASH          11
#define SCREEN_CUSTOM          12
#define SCREEN_COUNT           13  // Total number of screens

// Notification types
#define NOTIFICATION_INFO      0
#define NOTIFICATION_WARNING   1
#define NOTIFICATION_ERROR     2
#define NOTIFICATION_SUCCESS   3

// Display state structure
typedef struct {
    uint8_t currentScreen;        // Currently active screen
    uint8_t previousScreen;       // Previous screen to return to after notification
    bool displayNotification;     // Whether to show a notification
    uint8_t notificationType;     // Type of notification
    char notificationMessage[64]; // Notification message
    uint32_t notificationTimeout; // When notification should expire (millis)
    bool hasECUData;              // Whether we have ECU data
    bool refreshNeeded;           // Flag to force a refresh
    bool animationEnabled;        // Whether animations are enabled
    uint8_t animationFrame;       // Current animation frame
} DisplayState;

extern DisplayState displayState;

// Function declarations
void initDisplay();
void clearDisplays();
void updateDisplay();
void setDisplayScreen(uint8_t screenType);
void showNotification(uint8_t notificationType, const char* message, uint32_t timeout);
void clearNotification();

// Display routine declarations
void drawTaskbarText(Adafruit_SSD1306 &lcd, const char* text);
void renderScreen(uint8_t screenType);
void renderNotification();

// Declare the LCD objects as external so they can be accessed from other files
extern Adafruit_SSD1306 lcd1;
extern Adafruit_SSD1306 lcd2;

#endif // DISPLAY_H
