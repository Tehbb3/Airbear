#include "display.h"
#include "globals.h"
#include "display-routines.h"
#include "serialParser.h"
#include <ArduinoJson.h>
#include <Arduino.h>

// Define the LCD objects here so they can be accessed from other files
Adafruit_SSD1306 lcd1(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 lcd2(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, OLED_RESET);

// Initialize display state - using standard C++ initialization instead of C99 designated initializers
DisplayState displayState = {
    SCREEN_SPLASH,      // currentScreen
    SCREEN_MAIN,        // previousScreen
    false,              // displayNotification
    NOTIFICATION_INFO,  // notificationType
    "",                 // notificationMessage
    0,                  // notificationTimeout
    false,              // hasECUData
    true,               // refreshNeeded
    true,               // animationEnabled
    0                   // animationFrame
};

// Display state tracking
bool displayInitialized = false;
unsigned long lastAnimationUpdate = 0;
const unsigned long ANIMATION_INTERVAL = 100; // Update animations every 100ms

void initDisplay() {
    Serial.println("Initializing display...");

    // Initialize IIC
    Wire.begin(OLED_SDA, OLED_SCL);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!lcd1.begin(SSD1306_SWITCHCAPVCC, SCREEN_1_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed LCD1"));
    }
    if(!lcd2.begin(SSD1306_SWITCHCAPVCC, SCREEN_2_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed LCD2"));
    }

    lcd1.setRotation(2);
    lcd2.setRotation(2);

    // Clear the buffer
    lcd1.clearDisplay();
    lcd2.clearDisplay();

    // Flashbang to show display is working
    lcd1.fillScreen(SSD1306_WHITE);
    lcd2.fillScreen(SSD1306_WHITE);
    lcd1.display();
    lcd2.display();
    delay(100);
    
    clearDisplays();

    lcd1.display();
    lcd2.display();
    
    // Set initial state to splash screen
    displayState.currentScreen = SCREEN_SPLASH;
    displayState.refreshNeeded = true;
    
    // Trigger initial screen render
    // updateDisplay();
    
    displayInitialized = true;
    Serial.println("Display initialized successfully");
    
    // After 3 seconds, switch to the main screen
    // delay(3000);
    // setDisplayScreen(SCREEN_MAIN);

    // Moving to main screen via the splash screen itself thru animation so that other stuff can start sooner
}

void clearDisplays() {
    lcd1.clearDisplay();
    lcd2.clearDisplay();
}

void setDisplayScreen(uint8_t screenType) {
    if (screenType >= SCREEN_COUNT) {
        return;
    }
    
    displayState.previousScreen = displayState.currentScreen;
    displayState.currentScreen = screenType;
    displayState.refreshNeeded = true;
    
    // Clear notifications when changing screens
    if (displayState.displayNotification && screenType != SCREEN_NOTIFICATION) {
        displayState.displayNotification = false;
    }
}

void showNotification(uint8_t notificationType, const char* message, uint32_t timeout) {
    // Save the current screen so we can return to it
    if (displayState.currentScreen != SCREEN_NOTIFICATION) {
        displayState.previousScreen = displayState.currentScreen;
    }
    
    // Setup the notification
    displayState.notificationType = notificationType;
    strncpy(displayState.notificationMessage, message, sizeof(displayState.notificationMessage) - 1);
    displayState.notificationTimeout = millis() + timeout;
    displayState.displayNotification = true;
    displayState.currentScreen = SCREEN_NOTIFICATION;
    displayState.refreshNeeded = true;
}

void clearNotification() {
    displayState.displayNotification = false;
    displayState.currentScreen = displayState.previousScreen;
    displayState.refreshNeeded = true;
}

void updateDisplay() {
    unsigned long currentMillis = millis();
    
    // Update animation frames at a fixed rate
    if (currentMillis - lastAnimationUpdate >= ANIMATION_INTERVAL) {
        lastAnimationUpdate = currentMillis;
        displayState.animationFrame = (displayState.animationFrame + 1) % 60; // 60 animation frames (0-59)
        displayState.refreshNeeded = true;
    }
    
    // Check if notification timeout has expired
    if (displayState.displayNotification && currentMillis > displayState.notificationTimeout) {
        clearNotification();
    }
    
    // Force refresh for animated screens
    if (displayState.currentScreen == SCREEN_SPLASH || 
        displayState.currentScreen == SCREEN_LOADING || 
        displayState.currentScreen == SCREEN_NO_ECU_DATA) {
        displayState.refreshNeeded = true;
    }
    
    // Only update the display if something has changed
    if (!displayState.refreshNeeded) {
        return;
    }
    
    // Clear display before drawing
    clearDisplays();
    
    // Set hasECUData based on global connectivity status
    displayState.hasECUData = (hasConnectionToECU == 1);
    
    // If no ECU data available, override with no data screen
    uint8_t screenToRender = displayState.hasECUData ? 
                             displayState.currentScreen : 
                             SCREEN_NO_ECU_DATA;
                             
    // Exception: always show splash screen if that's the current screen
    if (displayState.currentScreen == SCREEN_SPLASH) {
        screenToRender = SCREEN_SPLASH;
    }
    
    // Render the appropriate screen
    renderScreen(screenToRender);
    
    // Reset the refresh flag
    displayState.refreshNeeded = false;
}

void renderScreen(uint8_t screenType) {
    switch (screenType) {
        case SCREEN_SPLASH:
            renderSplashScreen();
            break;
            
        case SCREEN_MAIN:
            renderMainScreen();
            break;

        case SCREEN_GRAPH:
            renderGraphScreen();
            break;

        case SCREEN_DIAGNOSTIC:
            renderDiagnosticScreen();
            break;
            
        case SCREEN_DIAGNOSTIC_2:
            renderDiagnosticScreenTwo();
            break;
            
        case SCREEN_ENGINE:
            renderEngineScreen();
            break;
            
        case SCREEN_TEMPS:
            renderTemperaturesScreen();
            break;
            
        case SCREEN_FUELING:
            renderFuelingScreen();
            break;
            
        case SCREEN_LOADING:
            renderLoadingScreen();
            break;
            
        case SCREEN_NOTIFICATION:
            renderNotificationScreen();
            break;
            
        case SCREEN_NO_ECU_DATA:
            renderNoECUDataScreen();
            break;
            
        case SCREEN_CUSTOM:
            renderCustomScreen();
            break;
            
        default:
            // Default to main screen if invalid screen type
            renderMainScreen();
            break;
    }
}

// ... (The rest of the file will be implemented in display-screens.cpp)
