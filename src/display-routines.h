#ifndef DISPLAY_ROUTINES_H
#define DISPLAY_ROUTINES_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Function declarations for display routines
void drawTaskbarText(Adafruit_SSD1306 &lcd, const char* text);

// Screen rendering functions
void renderSplashScreen();
void renderMainScreen();
void renderGraphScreen();
void renderDiagnosticScreen();
void renderDiagnosticScreenTwo();
void renderEngineScreen();
void renderTemperaturesScreen();
void renderFuelingScreen();
void renderLoadingScreen();
void renderNotificationScreen();
void renderNoECUDataScreen();
void renderCustomScreen();

// Helper functions
int getIntValueOrDefault(const char* key, int defaultValue);

// External variables needed from other files
extern unsigned long lastGoodDataTime;
extern uint32_t loopCounter;
extern bool hasConnectionToECU;

#endif // DISPLAY_ROUTINES_H
