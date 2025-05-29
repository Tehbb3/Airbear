// This file should be copied to the Arduino/libraries/TFT_eSPI folder

// Only ONE line below should be uncommented to define your setup.
// Comment all other lines out.

// #include <User_Setup.h>                  // Default setup is root library folder

// Setup for ESP32 C3 Super Mini with ST7789 display
// Pin connections:
//   GND --------- GND
//   VCC --------- 3.3V
//   SCL --------- 2
//   SDA --------- 4
//   RES --------- 0
//   DC ---------- 1
//   BLK---------  NC

#define USER_SETUP_LOADED 1

// ESP32C3 pins when using TFT_eSPI
#define TFT_MISO -1     // Not used
#define TFT_MOSI 4      // SDA pin
#define TFT_SCLK 2      // SCL pin  
#define TFT_CS   -1     // Not connected
#define TFT_DC   1      // Data/Command pin
#define TFT_RST  0      // Reset pin

// Display type
#define ST7789_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 240
#define TFT_INVERSION_ON
#define SPI_FREQUENCY  40000000
