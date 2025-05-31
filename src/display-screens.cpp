#include "display.h"
#include "globals.h"
#include "display-routines.h"
#include "serialParser.h"
#include <ArduinoJson.h>
#include <Arduino.h>

// Helper function to get int values from JSON with default
int getIntValueOrDefault(const char* key, int defaultValue) {
    if (readings_JSON.containsKey(key)) {
        return readings_JSON[key];
    }
    return defaultValue;
}

// Render the splash screen
void renderSplashScreen() {
    

    // Progress bar animation based on animation frame - smoother animation
    int maxWidth = 120;
    int progressWidth = map(displayState.animationFrame, 0, 59, 0, maxWidth*2);
    // if (progressWidth > maxWidth) progressWidth = maxWidth*2 - progressWidth; // Bounce effect
    if (progressWidth > maxWidth) { // 2nd half
        progressWidth = progressWidth - maxWidth; // Loop again
    }

    // Draw outline
    lcd1.drawRect(4, 36, maxWidth, 10, SSD1306_WHITE);
    
    // Ensure progress width is positive 
    progressWidth = max(0, min(progressWidth, maxWidth));
    
    // Fill with progress
    lcd1.fillRect(4, 36, progressWidth, 10, SSD1306_WHITE);

    

    // lcd1.setCursor(0, 0);
    // lcd1.setTextSize(1);
    // lcd1.println(F("Luxury."));
    // lcd1.println(F("Sports."));
    // lcd1.println(F("Vehicle."));


    lcd1.setTextColor(SSD1306_WHITE);

    // Draw version at bottom of screen
    lcd1.setCursor(0, 48);
    lcd1.print(F("v"));
    lcd1.println(FIRMWARE_VERSION);

    lcd1.display();

    // Second display
    // lcd2.setTextSize(2); 
    lcd2.setTextColor(SSD1306_WHITE);
    lcd2.setCursor(0, 24);
    lcd2.setTextSize(2);
    lcd2.println(F("Welcome"));
    lcd2.setTextSize(1);
    lcd2.println(F("Wsg"));
    lcd2.display();


    
    // If the animation is complete, leave splash screen
    // if (displayState.animationFrame >= 59) {
    if (millis() > 10000 || hasConnectionToECU == 1) {
        setDisplayScreen(SCREEN_MAIN);
    }


}

// Render the main screen with key engine parameters
void renderMainScreen() {
    // Read values from the JSON object
    int rpm = getIntValueOrDefault("rpm", 0);
    int coolant = getIntValueOrDefault("CLT", 0);
    int map = getIntValueOrDefault("MAP", 0);
    int afr = getIntValueOrDefault("AFR1", 0);
    int afr_target = getIntValueOrDefault("afr_target", 0);
    int tps = getIntValueOrDefault("TPS", 0);
    

        // Main parameters on display 1
    lcd1.setTextSize(1);
    lcd1.setCursor(0, 0);
    lcd1.print("RPM");
    lcd1.setTextSize(3);
    lcd1.setCursor(30, 0);
    lcd1.print(rpm);

    
    
    // TPS indicator
    lcd1.drawRect(120, 0, 8, ((tps / 100.0) * 64), SSD1306_WHITE);
    
    // Display taskbar with current screen name
    drawTaskbarText(lcd1, "MAIN");
    
    lcd1.display();
    
    // Second display - additional data
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 0);
    lcd2.print("AFR");
    lcd2.setTextSize(2);
    lcd2.setCursor(30, 0);
    lcd2.print(afr / 10.0, 1); // Convert to decimal
    
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 16);
    lcd2.print("FUEL");
    lcd2.setCursor(0, 24);
    lcd2.print("ERR");
    lcd2.setTextSize(2);
    lcd2.setCursor(30, 16);
    lcd2.print((afr - afr_target) / 10.0, 1); // Convert to decimal
    
    // Add more parameters to display 2
    int iat = getIntValueOrDefault("IAT", 0);
    int spark = getIntValueOrDefault("adv_deg", 0);
    
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 32);
    lcd2.print("CTS");
    lcd2.setTextSize(2);
    lcd2.setCursor(30, 32);
    lcd2.print(coolant);
    
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 48);
    lcd2.print("MAP");
    lcd2.setTextSize(2);
    lcd2.setCursor(30, 48);
    lcd2.print(map);
    
    lcd2.display();
}



void renderGraphScreen() {

    // Display taskbar with current screen name
    drawTaskbarText(lcd1, "Main2");
    
    // Read values from the JSON object
    int rpm = getIntValueOrDefault("rpm", 0);
    int coolant = getIntValueOrDefault("CLT", 0);
    int map = getIntValueOrDefault("MAP", 0);
    int afr = getIntValueOrDefault("AFR1", 0);
    int afr_target = getIntValueOrDefault("afr_target", 0);
    int tps = getIntValueOrDefault("TPS", 0);
    

    // Main parameters on display 1
    lcd1.setTextSize(1);
    lcd1.setCursor(0, 0);
    lcd1.print("RPM");
    lcd1.setTextSize(2);
    lcd1.setCursor(30, 0);
    lcd1.print(rpm);
    
    lcd1.setTextSize(1);
    lcd1.setCursor(0, 16);
    lcd1.print("CTS");
    lcd1.setTextSize(2);
    lcd1.setCursor(30, 16);
    lcd1.print(coolant);
    
    lcd1.setTextSize(1);
    lcd1.setCursor(0, 32);
    lcd1.print("MAP");
    lcd1.setTextSize(2);
    lcd1.setCursor(30, 32);
    lcd1.print(map);
    
    // TPS indicator
    lcd1.drawRect(120, 0, 8, ((tps / 100.0) * 64), SSD1306_WHITE);

    
    lcd1.display();
    
    // Second display - additional data
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 0);
    lcd2.print("AFR");
    lcd2.setTextSize(2);
    lcd2.setCursor(30, 0);
    lcd2.print(afr / 10.0, 1); // Convert to decimal
    
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 16);
    lcd2.print("TGT");
    lcd2.setTextSize(2);
    lcd2.setCursor(30, 16);
    lcd2.print(afr_target / 10.0, 1); // Convert to decimal
    
    // Add more parameters to display 2
    int iat = getIntValueOrDefault("IAT", 0);
    int spark = getIntValueOrDefault("adv_deg", 0);
    
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 32);
    lcd2.print("IAT");
    lcd2.setTextSize(2);
    lcd2.setCursor(30, 32);
    lcd2.print(iat);
    
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 48);
    lcd2.print("IGN");
    lcd2.setTextSize(2);
    lcd2.setCursor(30, 48);
    lcd2.print(spark);
    
    lcd2.display();
}





// Render diagnostic information screen
void renderDiagnosticScreen() {
    lcd1.setTextSize(1);
    lcd1.setCursor(0, 0);
    lcd1.println(F("DIAGNOSTIC"));
    
    lcd1.setCursor(0, 12);
    lcd1.print(F("LOOP: "));
    lcd1.print(loopCounter);
    
    lcd1.setCursor(0, 24);
    lcd1.print(F("MEM: "));
    lcd1.print(ESP.getFreeHeap());
    
    lcd1.setCursor(0, 36);
    lcd1.print(F("ECU: "));
    lcd1.print(hasConnectionToECU ? "CONNECTED" : "DISCONNECTED");
    
    lcd1.setCursor(0, 48);
    lcd1.print(F("WIFI: "));
    lcd1.print(WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");
    
    drawTaskbarText(lcd1, "DIAG");
    
    lcd1.display();
    
    // Second display - system stats
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 0);
    lcd2.println(F("SYSTEM INFO"));
    
    lcd2.setCursor(0, 12);
    lcd2.print(F("Req Queue: "));
    lcd2.println(serialECURequestQueueSize);
    
    lcd2.setCursor(0, 24);
    lcd2.print(F("SSID: "));
    lcd2.println(WiFi.SSID());
    
    lcd2.setCursor(0, 36);
    lcd2.print(F("RSSI: "));
    lcd2.print(WiFi.RSSI());
    lcd2.println(F(" dBm"));
    
    lcd2.setCursor(0, 48);
    lcd2.print(F("UPTIME: "));
    lcd2.print(millis() / 1000 / 60); // Minutes
    lcd2.println(F("m"));
    
    lcd2.display();
}

void renderDiagnosticScreenTwo() {
    lcd1.setTextSize(1);
    lcd1.setCursor(0, 0);
    lcd1.println(F("DIAGNOSTIC"));

    lcd1.setCursor(0, 12);
    lcd1.print(F("LOOP: "));
    lcd1.print(loopCounter);

    lcd1.setCursor(0, 24);
    lcd1.print(F("MEM: "));
    lcd1.print(ESP.getFreeHeap());

    lcd1.setCursor(0, 36);
    lcd1.print(F("ECU: "));
    lcd1.print(hasConnectionToECU ? "CONNECTED" : "DISCONNECTED");

    lcd1.setCursor(0, 48);
    lcd1.print(F("WIFI: "));
    lcd1.print(WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");

    drawTaskbarText(lcd1, "DIAG TWO");

    lcd1.display();

    // Second display - system stats
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 0);
    lcd2.println(F("INFO"));

    lcd2.setCursor(0, 12);
    lcd2.print(F("Error Code: "));
    lcd2.println(getIntValueOrDefault("error_codes", 0));

    lcd2.setCursor(0, 24);
    lcd2.print(F("Launch Crect: "));
    lcd2.println(getIntValueOrDefault("launch_correction", 0));

    lcd2.setCursor(0, 36);
    lcd2.print(F("Idle Duty: "));
    lcd2.println(getIntValueOrDefault("idle_duty", 0));

    lcd2.setCursor(0, 48);
    lcd2.print(F("Gear: "));
    lcd2.println(getIntValueOrDefault("current_gear", 0));

    lcd2.display();
}



// Render detailed engine parameters
void renderEngineScreen() {
    int rpm = getIntValueOrDefault("rpm", 0);
    int tps = getIntValueOrDefault("TPS", 0);
    int map = getIntValueOrDefault("MAP", 0);
    int maf = getIntValueOrDefault("MAF", 0);
    int ve = getIntValueOrDefault("VE1", 0);
    int pw1 = getIntValueOrDefault("PW1", 0);
    
    // Main engine parameters
    lcd1.setTextSize(1);
    lcd1.setCursor(0, 0);
    lcd1.println(F("ENGINE"));
    
    // RPM bar
    int rpmBarWidth = (rpm * 100) / 8000; // Scale RPM from 0-8000 to 0-100 pixels
    lcd1.drawRect(0, 12, 100, 8, SSD1306_WHITE);
    lcd1.fillRect(0, 12, rpmBarWidth, 8, SSD1306_WHITE);
    lcd1.setCursor(105, 12);
    lcd1.print(rpm);
    
    // Draw TPS and MAP in rows
    lcd1.setCursor(0, 24);
    lcd1.print(F("TPS: "));
    lcd1.print(tps);
    lcd1.print(F("%"));
    
    lcd1.setCursor(0, 36);
    lcd1.print(F("MAP: "));
    lcd1.print(map);
    lcd1.print(F(" kPa"));
    
    lcd1.setCursor(0, 48);
    lcd1.print(F("VE: "));
    lcd1.print(ve);
    lcd1.print(F("%"));
    
    drawTaskbarText(lcd1, "ENGINE");
    
    lcd1.display();
    
    // Second display - more engine params
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 0);
    lcd2.println(F("ENGINE DETAIL"));
    
    // PW bar (pulse width)
    int pwBarWidth = pw1 / 200; // Convert PW from microseconds (0-20000) to pixel width (0-100)
    lcd2.drawRect(0, 12, 100, 8, SSD1306_WHITE);
    lcd2.fillRect(0, 12, pwBarWidth, 8, SSD1306_WHITE);
    lcd2.setCursor(105, 12);
    lcd2.print(pw1 / 1000.0, 1); // Convert to ms
    
    // MAF and other readings
    lcd2.setCursor(0, 24);
    lcd2.print(F("MAF: "));
    lcd2.print(maf);
    lcd2.print(F(" g/s"));
    
    int load = getIntValueOrDefault("engine_load", 0);
    lcd2.setCursor(0, 36);
    lcd2.print(F("LOAD: "));
    lcd2.print(load);
    lcd2.print(F("%"));
    
    int engineStatus = getIntValueOrDefault("engine_status", 0);
    lcd2.setCursor(0, 48);
    lcd2.print(F("STATUS: "));
    
    // Engine status interpretation
    const char* status = "UNKNOWN";
    switch(engineStatus) {
        case 0: status = "OFF"; break;
        case 1: status = "CRANKING"; break;
        case 2: status = "RUNNING"; break;
        case 3: status = "WARMUP"; break;
    }
    lcd2.print(status);
    
    lcd2.display();
}

// Render temperature data
void renderTemperaturesScreen() {
    int coolant = getIntValueOrDefault("CLT", 0);
    int iat = getIntValueOrDefault("IAT", 0);
    int oilTemp = getIntValueOrDefault("oilTemp", 0);
    int fuelTemp = getIntValueOrDefault("fuelTemp", 0);
    int ambientTemp = getIntValueOrDefault("ambientTemp", 0);
    int batteryVoltage = getIntValueOrDefault("batt_v", 0);
    
    // Temperature screen
    lcd1.setTextSize(1);
    lcd1.setCursor(0, 0);
    lcd1.println(F("TEMPERATURES"));
    
    // Coolant temp with bar
    int coolantBarWidth = map(coolant, 0, 120, 0, 80);
    lcd1.drawRect(0, 12, 80, 8, SSD1306_WHITE);
    lcd1.fillRect(0, 12, coolantBarWidth, 8, coolant > 100 ? SSD1306_INVERSE : SSD1306_WHITE);
    lcd1.setCursor(85, 12);
    lcd1.print(coolant);
    lcd1.print(F("C"));
    
    // IAT with bar
    int iatBarWidth = map(iat, 0, 100, 0, 80);
    lcd1.drawRect(0, 24, 80, 8, SSD1306_WHITE);
    lcd1.fillRect(0, 24, iatBarWidth, 8, SSD1306_WHITE);
    lcd1.setCursor(85, 24);
    lcd1.print(iat);
    lcd1.print(F("C"));
    
    // Oil temp with bar if available
    int oilBarWidth = map(oilTemp, 0, 150, 0, 80);
    lcd1.drawRect(0, 36, 80, 8, SSD1306_WHITE);
    lcd1.fillRect(0, 36, oilBarWidth, 8, oilTemp > 120 ? SSD1306_INVERSE : SSD1306_WHITE);
    lcd1.setCursor(85, 36);
    lcd1.print(oilTemp);
    lcd1.print(F("C"));
    
    // Battery voltage
    lcd1.setCursor(0, 48);
    lcd1.print(F("BATT: "));
    lcd1.print(batteryVoltage / 10.0, 1); // Convert to actual voltage
    lcd1.print(F("V"));
    
    drawTaskbarText(lcd1, "TEMPS");
    
    lcd1.display();
    
    // Second display - more temp data and analysis
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 0);
    lcd2.println(F("TEMPERATURE DETAIL"));
    
    // Delta between IAT and CLT
    int tempDelta = coolant - iat;
    lcd2.setCursor(0, 12);
    lcd2.print(F("∆T: "));
    lcd2.print(tempDelta);
    lcd2.print(F("C"));
    
    // Fuel temperature if available
    lcd2.setCursor(0, 24);
    lcd2.print(F("FUEL: "));
    lcd2.print(fuelTemp);
    lcd2.print(F("C"));
    
    // Ambient temp if available
    lcd2.setCursor(0, 36);
    lcd2.print(F("AMB: "));
    lcd2.print(ambientTemp);
    lcd2.print(F("C"));
    
    // Heat efficiency metric (made up for example)
    lcd2.setCursor(0, 48);
    lcd2.print(F("HEAT EFF: "));
    
    // Simple heat efficiency indicator
    const char* heatStatus = "NORMAL";
    if (coolant < 60) heatStatus = "COLD";
    else if (coolant > 100) heatStatus = "HOT";
    lcd2.print(heatStatus);
    
    lcd2.display();
}

// Render fueling information
void renderFuelingScreen() {
    int afr = getIntValueOrDefault("AFR1", 0);
    int afr_target = getIntValueOrDefault("afr_target", 0);
    int pw1 = getIntValueOrDefault("PW1", 0);
    int tps = getIntValueOrDefault("TPS", 0);
    int ve = getIntValueOrDefault("VE1", 0);
    int map = getIntValueOrDefault("MAP", 0);
    
    // Calculate AFR error
    float afrActual = afr / 10.0;
    float afrTargetVal = afr_target / 10.0;
    float afrError = afrActual - afrTargetVal;
    
    // Fueling screen
    lcd1.setTextSize(1);
    lcd1.setCursor(0, 0);
    lcd1.println(F("FUELING"));
    
    // AFR with bar - green when close to target, red when far
    int afrBarCenter = 64; // Center position
    int afrBarOffset = abs(afrError * 10) * 4 / 3; // Scale error to pixels (0-30 to 0-40)
    
    // Draw bar reference line
    lcd1.drawFastHLine(24, 16, 80, SSD1306_WHITE);
    
    // Draw center marker
    lcd1.drawFastVLine(afrBarCenter, 12, 8, SSD1306_WHITE);
    
    // Draw AFR indicator - position based on error
    int afrIndicator = afrBarCenter + (afrError * 10);
    afrIndicator = constrain(afrIndicator, 24, 104); // Stay within bar
    lcd1.fillRect(afrIndicator-2, 12, 5, 8, SSD1306_WHITE);
    
    // AFR values
    lcd1.setCursor(0, 12);
    lcd1.print(afrActual, 1);
    
    lcd1.setCursor(105, 12);
    lcd1.print(afrTargetVal, 1);
    
    // Pulse width
    lcd1.setCursor(0, 24);
    lcd1.print(F("PW: "));
    lcd1.print(pw1 / 1000.0, 1); // ms
    lcd1.print(F("ms"));
    
    // VE
    lcd1.setCursor(0, 36);
    lcd1.print(F("VE: "));
    lcd1.print(ve);
    lcd1.print(F("%"));
    
    // MAP
    lcd1.setCursor(0, 48);
    lcd1.print(F("MAP: "));
    lcd1.print(map);
    lcd1.print(F("kPa"));
    
    drawTaskbarText(lcd1, "FUEL");
    
    lcd1.display();
    
    // Second display - more fueling data
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 0);
    lcd2.println(F("FUEL DETAIL"));
    
    // Lambda value
    float lambda = afrActual / 14.7;
    lcd2.setCursor(0, 12);
    lcd2.print(F("λ: "));
    lcd2.print(lambda, 2);
    
    // Injector duty cycle
    int rpm = getIntValueOrDefault("rpm", 0);
    // Simplified IDC calculation - adjust for your engine
    float idc = 0;
    if (rpm > 0) {
        idc = (pw1 / 1000.0) * (rpm / 60.0 * 2) / 1000.0 * 100.0; // Basic IDC calculation
        idc = constrain(idc, 0, 100);
    }
    
    lcd2.setCursor(0, 24);
    lcd2.print(F("IDC: "));
    lcd2.print(idc, 1);
    lcd2.print(F("%"));
    
    // Fuel pressure if available
    int fuelPressure = getIntValueOrDefault("fuelPressure", 0);
    lcd2.setCursor(0, 36);
    lcd2.print(F("FUEL P: "));
    lcd2.print(fuelPressure);
    lcd2.print(F("kPa"));
    
    // Mixture status
    lcd2.setCursor(0, 48);
    lcd2.print(F("MIX: "));
    
    // Simple mixture status based on AFR error
    const char* mixStatus = "OPTIMAL";
    if (afrError < -0.5) mixStatus = "RICH";
    else if (afrError > 0.5) mixStatus = "LEAN";
    lcd2.print(mixStatus);
    
    lcd2.display();
}

// Render loading/progress screen
void renderLoadingScreen() {
    lcd1.setTextSize(1);
    lcd1.setCursor(0, 0);
    lcd1.println(F("LOADING DATA"));
    
    // Progress bar animation based on animation frame - improved animation
    int maxWidth = 120;
    int progressWidth = map(displayState.animationFrame, 0, 59, 0, maxWidth*2);
    if (progressWidth > maxWidth) progressWidth = maxWidth*2 - progressWidth; // Bounce effect
    
    // Draw outline
    lcd1.drawRect(4, 20, maxWidth, 10, SSD1306_WHITE);
    
    // Ensure progress width is positive
    progressWidth = max(0, min(progressWidth, maxWidth));
    
    // Fill with progress
    lcd1.fillRect(4, 20, progressWidth, 10, SSD1306_WHITE);
    
    lcd1.setCursor(0, 40);
    lcd1.println(F("Please wait..."));
    
    lcd1.display();
    
    // Second display
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 0);
    lcd2.println(F("SYSTEM STATUS"));
    
    lcd2.setCursor(0, 16);
    lcd2.print(F("Connected: "));
    lcd2.println(hasConnectionToECU ? "YES" : "NO");
    
    lcd2.setCursor(0, 32);
    lcd2.print(F("SSID: "));
    lcd2.println(WiFi.SSID());
    
    lcd2.setCursor(0, 48);
    lcd2.print(F("IP: "));
    lcd2.println(WiFi.localIP().toString());
    
    lcd2.display();
}

// Render notification screen
void renderNotificationScreen() {
    lcd1.setTextSize(1);
    
    // Set color based on notification type
    uint16_t textColor = SSD1306_WHITE;
    const char* typeText = "INFO";
    
    switch (displayState.notificationType) {
        case NOTIFICATION_WARNING:
            typeText = "WARNING";
            break;
        case NOTIFICATION_ERROR:
            typeText = "ERROR";
            // Flash the text for errors
            if (displayState.animationFrame % 10 > 5) {
                textColor = SSD1306_BLACK; // Flash effect
            }
            break;
        case NOTIFICATION_SUCCESS:
            typeText = "SUCCESS";
            break;
        default:
            break;
    }
    
    lcd1.setTextColor(textColor);
    
    // Draw notification box
    lcd1.drawRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, textColor);
    lcd1.setCursor(4, 4);
    lcd1.print(typeText);
    
    // Draw separator line
    lcd1.drawFastHLine(0, 14, DISPLAY_WIDTH, textColor);
    
    // Draw message - handle word wrapping
    lcd1.setCursor(4, 20);
    
    // Simple word wrap implementation
    String message = displayState.notificationMessage;
    int cursorY = 20;
    
    // Remove text that would wrap
    int charCount = 0;
    String line = "";
    
    for (unsigned int i = 0; i < message.length(); i++) {
        line += message[i];
        charCount++;
        
        // Check for natural breaks or forced line breaks
        if (message[i] == ' ' || message[i] == '\n' || charCount >= 20) {
            if (cursorY < 50) { // Ensure we don't go past bottom of screen
                lcd1.setCursor(4, cursorY);
                lcd1.print(line);
                cursorY += 10; // Line height
            }
            line = "";
            charCount = 0;
        }
    }
    
    // Print any remaining text
    if (line.length() > 0 && cursorY < 50) {
        lcd1.setCursor(4, cursorY);
        lcd1.print(line);
    }
    
    // Draw timeout countdown
    unsigned long timeLeft = 0;
    if (displayState.notificationTimeout > millis()) {
        timeLeft = (displayState.notificationTimeout - millis()) / 1000; // Seconds remaining
    }
    
    lcd1.setCursor(100, 4);
    lcd1.print(timeLeft);
    
    lcd1.display();
    
    // Keep second display showing background info
    renderScreen(displayState.previousScreen);
}

// Render no ECU data screen
void renderNoECUDataScreen() {

    lcd1.setTextSize(1);
    lcd1.setTextColor(SSD1306_WHITE);
    lcd1.setCursor(0, 20);
    lcd1.println(F("Verify ECU connection"));

    drawTaskbarText(lcd1, "NO DATA");
    
    lcd1.display();
    
    // Second display - troubleshooting info
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 0);
    lcd2.println(F("No Connection"));
    
    lcd2.setCursor(0, 12);
    lcd2.print(F("Serial Queue: "));
    lcd2.println(serialECURequestQueueSize);

    
    lcd2.setCursor(0, 32);
    lcd2.println(F("Last good data: "));
    
    // Calculate time since last data
    unsigned long dataAge = 0;
    if (lastGoodDataTime > 0) {
        dataAge = (millis() - lastGoodDataTime) / 1000; // seconds
    }
    
    if (dataAge < 60) {
        lcd2.print(dataAge);
        lcd2.print(F("s ago"));
    } else if (dataAge < 3600) {
        lcd2.print(dataAge / 60);
        lcd2.print(F("m ago"));
    } else {
        lcd2.print(F("long ago"));
    }
    
    lcd2.display();
}

// Render custom screen template
void renderCustomScreen() {
    lcd1.setTextSize(1);
    lcd1.setCursor(0, 0);
    lcd1.println(F("CUSTOM SCREEN"));

    drawTaskbarText(lcd1, "CUSTOM");

    lcd1.display();

    // Second display
    lcd2.setTextSize(1);
    lcd2.setCursor(0, 0);
    lcd2.println(F("CUSTOM DATA"));



    lcd2.display();
}
