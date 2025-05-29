#include "display.h"
#include "globals.h"
#include <ArduinoJson.h>
#include <Arduino.h>

Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, OLED_RESET);


// Initialize the display
// Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_SDA, TFT_SCL, TFT_RST);
// Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Display state tracking
bool displayInitialized = false;
unsigned long lastDisplayUpdate = 0;
const unsigned long DISPLAY_UPDATE_INTERVAL = 100; // Update display every 100ms

void initDisplay() {
    Serial.println("Initializing display...");

    // Initialize IIC
    Wire.begin(2,3);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        // for(;;); // Don't proceed, loop forever
    }

    display.display();
    // delay(500); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();

    // Draw a single pixel in white
    // display.drawPixel(10, 10, SSD1306_WHITE);

    // Flashbang
    display.fillScreen(SSD1306_WHITE);

    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();
  
    // Display splash screen
    displaySplashScreen();

    displayInitialized = true;
    Serial.println("Display initialized successfully");
}

void clearDisplay() {
    display.clearDisplay();
}

void displaySplashScreen() {
    clearDisplay();
  
    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Starting.."));
    display.display();      // Show initial text

    display.setCursor(4, 24);
    display.setTextSize(1);
    display.println(F("(Maybe)"));

    display.display();


  // Display product name
//   tft.setCursor(30, 80);
//   tft.println(PRODUCT_NAME);
  
//   // Display version
//   tft.setTextSize(1);
//   tft.setTextColor(ST77XX_WHITE);
//   tft.setCursor(80, 120);
//   tft.print("v");
//   tft.println(FIRMWARE_VERSION);iiiiiii
  
//   // Display initialization message
//   tft.setCursor(60, 160);
//   tft.println("Initializing...");
  
//   delay(2000); // Show splash screen for 2 seconds
}

void displayNoData() {
    clearDisplay();
  
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(24, 0);
    display.println(F("NO DATA"));
    display.setCursor(4, 24);
    display.setTextSize(1);
    display.println(F("CHECK ECU CONNECTION"));

    display.display();
}

void drawGauge(int x, int y, int radius, int value, int minValue, int maxValue, 
              const char* label, const char* units, uint16_t gaugeColor) {
  
  // Normalize value within min/max range
  value = constrain(value, minValue, maxValue);
  int angle = map(value, minValue, maxValue, 45, 315);
  
  // Draw gauge background
  display.fillCircle(x, y, radius, SSD1306_BLACK);
  display.drawCircle(x, y, radius, SSD1306_WHITE);
  
  // Draw gauge label
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x - (strlen(label) * 3), y - 15);
  display.print(label);
  
  // Draw min/max values
  display.setCursor(x - radius + 5, y + 5);
  display.print(minValue);
  display.setCursor(x + radius - 15, y + 5);
  display.print(maxValue);
  
  // Draw value
  display.setTextSize(2);
  display.setCursor(x - 20, y + 20);
  display.print(value);
  display.setTextSize(1);
  display.print(units);
  
  // Draw gauge needle
  float radian = (angle - 90) * 0.0174533; // Convert to radians
  int needleX = x + int(cos(radian) * (radius - 10));
  int needleY = y + int(sin(radian) * (radius - 10));
  display.drawLine(x, y, needleX, needleY, gaugeColor);
  display.fillCircle(x, y, 5, gaugeColor);
}

void updateDisplay() {
//   // Limit update frequency to avoid excessive redrawing
    if (millis() - lastDisplayUpdate < DISPLAY_UPDATE_INTERVAL) {
        return;
    }  
    lastDisplayUpdate = millis();
    
    // Clear the display
    clearDisplay();
    
    // Read values from the JSON object
    int rpm = 0;
    int coolant = 0;
    int map = 0;
    int afr = 0;
    int afr_target = 0;
    int tps = 0;
    
    
    // Try to get the data from the readings_JSON object
    if (readings_JSON.containsKey("rpm")) {
        rpm = readings_JSON["rpm"];
    } else { 
        rpm = -255;
    }
    
    if (readings_JSON.containsKey("CLT")) {
        coolant = readings_JSON["CLT"];
    } else {
        coolant = -255;
    }

    if (readings_JSON.containsKey("MAP")) {
        map = readings_JSON["MAP"];
    } else {
        map = -255;
    }

    if (readings_JSON.containsKey("AFR1")) {
        afr = readings_JSON["AFR1"];
    } else { 
        afr = -255;
    }

    if (readings_JSON.containsKey("afr_target")) {
        afr_target = readings_JSON["afr_target"];
    } else {
        afr_target = -255;
    }

    if (readings_JSON.containsKey("TPS")) {
        tps = readings_JSON["TPS"];
    } else {
        tps = -255;
    }


    // Serial.println(coolant);
    // Serial.println(rpm);


display.setTextSize(2);

display.setTextSize(1);
display.setCursor(0,0);
display.print("RPM");
display.setTextSize(2);
display.setCursor(30,0);
display.print(rpm);

display.setTextSize(1);
display.setCursor(0, 16);
display.print("CTS");
display.setTextSize(2);
display.setCursor(30, 16);
display.print(coolant);


display.setTextSize(1);
display.setCursor(0, 32);
display.print("MAP");
display.setTextSize(2);
display.setCursor(30, 32);
display.print(map);

display.setTextSize(1);
display.setCursor(0, 48);
display.print("Fuel");
display.setCursor(0, 56);
display.print("Err");
display.setTextSize(2);
display.setCursor(30, 48);
display.print(afr - afr_target);


// TPS indicator
display.drawRect(120, 0, 8, ((tps / 100) * 64), SSD1306_WHITE);


display.display();


//   // Draw RPM gauge in the top half
//   drawGauge(120, 80, 70, rpm, 0, 8000, "RPM", "", SSD1306_WHITE);
  
//   // Draw coolant temperature gauge in the bottom half
//   drawGauge(120, 180, 50, coolant, 0, 120, "COOLANT", "C", SSD1306_WHITE);
}
