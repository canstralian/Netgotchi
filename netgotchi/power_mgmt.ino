
// Power Management System
// Battery monitoring, low power modes, and power optimization

struct PowerStatus {
  float batteryVoltage;
  int batteryPercentage;
  bool isCharging;
  bool lowPowerMode;
  unsigned long powerOnTime;
  unsigned long lastBatteryCheck;
};

PowerStatus powerStatus;

// Power management settings
const float BATTERY_MIN_VOLTAGE = 3.2;  // Minimum safe voltage
const float BATTERY_MAX_VOLTAGE = 4.2;  // Maximum voltage when fully charged
const int LOW_BATTERY_THRESHOLD = 20;   // Percentage to trigger low power mode
const int CRITICAL_BATTERY_THRESHOLD = 10; // Percentage to trigger emergency shutdown

// Low power mode settings
bool emergencyMode = false;
unsigned long lastLowPowerCheck = 0;
const unsigned long LOW_POWER_CHECK_INTERVAL = 30000; // Check every 30 seconds

void initPowerManagement() {
  powerStatus.powerOnTime = millis();
  powerStatus.lowPowerMode = false;
  powerStatus.lastBatteryCheck = 0;
  
  Serial.println("Power Management System initialized");
  
  // Initial battery reading
  updateBatteryStatus();
}

void updateBatteryStatus() {
  // Read battery voltage from analog pin (adjust for your hardware)
  int analogValue = analogRead(A0);
  
  #ifdef ESP32
    powerStatus.batteryVoltage = (analogValue * 3.3) / 4095.0 * 2; // Voltage divider compensation
  #else
    powerStatus.batteryVoltage = (analogValue * 3.3) / 1024.0 * 2; // ESP8266
  #endif
  
  // Calculate battery percentage
  powerStatus.batteryPercentage = map(
    powerStatus.batteryVoltage * 100, 
    BATTERY_MIN_VOLTAGE * 100, 
    BATTERY_MAX_VOLTAGE * 100, 
    0, 100
  );
  powerStatus.batteryPercentage = constrain(powerStatus.batteryPercentage, 0, 100);
  
  // Detect charging (simple heuristic - voltage increasing)
  static float lastVoltage = 0;
  powerStatus.isCharging = (powerStatus.batteryVoltage > lastVoltage + 0.1);
  lastVoltage = powerStatus.batteryVoltage;
  
  powerStatus.lastBatteryCheck = millis();
  
  Serial.println("Battery: " + String(powerStatus.batteryPercentage) + "% (" + 
                 String(powerStatus.batteryVoltage) + "V)");
}

void managePowerMode() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastLowPowerCheck >= LOW_POWER_CHECK_INTERVAL) {
    lastLowPowerCheck = currentTime;
    updateBatteryStatus();
    
    // Check for low battery conditions
    if (powerStatus.batteryPercentage <= CRITICAL_BATTERY_THRESHOLD && !powerStatus.isCharging) {
      enterEmergencyMode();
    }
    else if (powerStatus.batteryPercentage <= LOW_BATTERY_THRESHOLD && !powerStatus.isCharging) {
      enterLowPowerMode();
    }
    else if (powerStatus.batteryPercentage > LOW_BATTERY_THRESHOLD + 10) {
      exitLowPowerMode(); // Exit with hysteresis
    }
  }
}

void enterLowPowerMode() {
  if (powerStatus.lowPowerMode) return; // Already in low power mode
  
  powerStatus.lowPowerMode = true;
  Serial.println("Entering Low Power Mode");
  
  // Reduce scanning frequency
  intervalScan = intervalScan * 2; // Double scan interval
  intervalPing = intervalPing * 2; // Double ping interval
  
  // Reduce display brightness (if supported by your display)
  #ifdef HAS_BRIGHTNESS_CONTROL
    setBrightness(50); // 50% brightness
  #endif
  
  // Disable non-essential features
  sounds = false; // Disable sounds to save power
  
  // Show low power indicator
  displayPrintln("LOW POWER MODE");
  displayDisplay();
}

void exitLowPowerMode() {
  if (!powerStatus.lowPowerMode) return; // Not in low power mode
  
  powerStatus.lowPowerMode = false;
  Serial.println("Exiting Low Power Mode");
  
  // Restore normal scanning frequency
  intervalScan = 10000; // Restore default values
  intervalPing = 5000;
  
  // Restore display brightness
  #ifdef HAS_BRIGHTNESS_CONTROL
    setBrightness(100);
  #endif
  
  // Re-enable features
  sounds = true;
}

void enterEmergencyMode() {
  if (emergencyMode) return;
  
  emergencyMode = true;
  Serial.println("EMERGENCY MODE: Critical battery level");
  
  // Minimal operations only
  intervalScan = 60000; // Scan once per minute
  intervalPing = 30000; // Ping every 30 seconds
  
  // Disable all non-essential features
  sounds = false;
  webInterface = false;
  
  // Show emergency warning
  displayClearDisplay();
  displayPrintln("EMERGENCY MODE");
  displayPrintln("BATTERY CRITICAL");
  displayPrintln("CHARGING REQUIRED");
  displayDisplay();
  
  // Consider automatic shutdown after some time
  scheduleEmergencyShutdown();
}

void scheduleEmergencyShutdown() {
  // Schedule shutdown in 10 minutes if still not charging
  static unsigned long shutdownTimer = 0;
  
  if (shutdownTimer == 0) {
    shutdownTimer = millis() + 600000; // 10 minutes
  }
  
  if (millis() > shutdownTimer && !powerStatus.isCharging) {
    Serial.println("Emergency shutdown due to critical battery");
    displayClearDisplay();
    displayPrintln("SHUTTING DOWN");
    displayPrintln("LOW BATTERY");
    displayDisplay();
    delay(5000);
    ESP.deepSleep(0); // Deep sleep until reset
  }
}

String getPowerReport() {
  String report = "=== POWER STATUS ===\n";
  report += "Battery: " + String(powerStatus.batteryPercentage) + "%\n";
  report += "Voltage: " + String(powerStatus.batteryVoltage, 2) + "V\n";
  report += "Charging: " + String(powerStatus.isCharging ? "YES" : "NO") + "\n";
  report += "Power Mode: " + String(powerStatus.lowPowerMode ? "LOW POWER" : "NORMAL") + "\n";
  report += "Emergency: " + String(emergencyMode ? "YES" : "NO") + "\n";
  
  unsigned long uptime = millis() - powerStatus.powerOnTime;
  report += "Uptime: " + String(uptime / 3600000) + "h " + 
            String((uptime % 3600000) / 60000) + "m\n";
  
  return report;
}

void optimizePowerConsumption() {
  // Dynamic power optimization based on activity
  static unsigned long lastActivity = millis();
  
  // If no threats detected for a while, reduce scanning
  if (!honeypotTriggered && !evilTwinDetected && 
      millis() - lastActivity > 300000) { // 5 minutes of quiet
    intervalScan = min(intervalScan * 1.5, 60000); // Gradually increase interval
  } else {
    lastActivity = millis();
    intervalScan = 10000; // Reset to normal
  }
  
  // CPU frequency scaling (ESP32 only)
  #ifdef ESP32
    if (powerStatus.lowPowerMode) {
      setCpuFrequencyMhz(80); // Reduce from 240MHz to 80MHz
    } else {
      setCpuFrequencyMhz(240); // Full speed
    }
  #endif
}
