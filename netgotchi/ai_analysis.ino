
// AI-Powered Analysis Module
// Implements basic behavioral analysis and anomaly detection

struct DeviceBehavior {
  IPAddress ip;
  unsigned long lastSeen;
  unsigned long firstSeen;
  int connectionFrequency;
  int dataTransferPattern;
  bool isSuspicious;
  float riskScore;
};

DeviceBehavior deviceProfiles[256];
int profileCount = 0;

// Behavioral patterns tracking
unsigned long trafficBaseline = 0;
unsigned long lastTrafficCheck = 0;
bool anomalyDetected = false;
String anomalyReason = "";

void initAIAnalysis() {
  Serial.println("Initializing AI Analysis Module");
  profileCount = 0;
  trafficBaseline = 0;
}

void analyzeDeviceBehavior(IPAddress ip, bool isConnected) {
  int profileIndex = findOrCreateDeviceProfile(ip);
  DeviceBehavior* profile = &deviceProfiles[profileIndex];
  
  unsigned long currentTime = millis();
  
  if (isConnected) {
    if (profile->firstSeen == 0) {
      profile->firstSeen = currentTime;
    }
    profile->lastSeen = currentTime;
    profile->connectionFrequency++;
    
    // Calculate risk score based on behavior
    calculateRiskScore(profile);
    
    // Check for suspicious patterns
    detectSuspiciousActivity(profile);
  }
}

int findOrCreateDeviceProfile(IPAddress ip) {
  // Find existing profile
  for (int i = 0; i < profileCount; i++) {
    if (deviceProfiles[i].ip == ip) {
      return i;
    }
  }
  
  // Create new profile if space available
  if (profileCount < 256) {
    deviceProfiles[profileCount].ip = ip;
    deviceProfiles[profileCount].firstSeen = 0;
    deviceProfiles[profileCount].lastSeen = 0;
    deviceProfiles[profileCount].connectionFrequency = 0;
    deviceProfiles[profileCount].isSuspicious = false;
    deviceProfiles[profileCount].riskScore = 0.0;
    return profileCount++;
  }
  
  return -1; // No space available
}

void calculateRiskScore(DeviceBehavior* profile) {
  float score = 0.0;
  unsigned long currentTime = millis();
  
  // Factor 1: New device penalty
  if (currentTime - profile->firstSeen < 3600000) { // Less than 1 hour
    score += 0.3;
  }
  
  // Factor 2: Connection frequency anomaly
  if (profile->connectionFrequency > 100) { // Very frequent connections
    score += 0.4;
  }
  
  // Factor 3: Time-based anomaly (connections at unusual hours)
  int hour = (timeClient.getEpochTime() % 86400) / 3600;
  if (hour < 6 || hour > 22) { // Late night/early morning activity
    score += 0.2;
  }
  
  // Factor 4: Rapid successive connections
  if (currentTime - profile->lastSeen < 5000 && profile->connectionFrequency > 5) {
    score += 0.5;
  }
  
  profile->riskScore = min(score, 1.0);
  
  // Mark as suspicious if risk score is high
  if (profile->riskScore > 0.7) {
    profile->isSuspicious = true;
    triggerSecurityAlert("High Risk Device Detected", profile->ip);
  }
}

void detectSuspiciousActivity(DeviceBehavior* profile) {
  // Pattern 1: Rapid connection attempts (potential brute force)
  if (profile->connectionFrequency > 50 && 
      (millis() - profile->firstSeen) < 300000) { // 50 attempts in 5 minutes
    triggerSecurityAlert("Potential Brute Force Attack", profile->ip);
  }
  
  // Pattern 2: Connection outside normal hours repeatedly
  int hour = (timeClient.getEpochTime() % 86400) / 3600;
  if ((hour < 6 || hour > 22) && profile->connectionFrequency > 10) {
    triggerSecurityAlert("Suspicious After-Hours Activity", profile->ip);
  }
}

void triggerSecurityAlert(String alertType, IPAddress ip) {
  Serial.println("SECURITY ALERT: " + alertType + " from " + ip.toString());
  
  // Visual alert
  honeypotTriggered = true;
  
  // Audio alert if enabled
  if (sounds) {
    playAlert();
  }
  
  // Log the incident
  logSecurityIncident(alertType, ip);
}

void logSecurityIncident(String incidentType, IPAddress ip) {
  // Simple logging - in production this could write to SPIFFS/LittleFS
  Serial.println("[SECURITY LOG] " + String(timeClient.getFormattedTime()) + 
                 " - " + incidentType + " - " + ip.toString());
}

String getAIAnalysisReport() {
  String report = "AI Analysis Report:\n";
  report += "Monitored Devices: " + String(profileCount) + "\n";
  
  int suspiciousCount = 0;
  for (int i = 0; i < profileCount; i++) {
    if (deviceProfiles[i].isSuspicious) {
      suspiciousCount++;
    }
  }
  
  report += "Suspicious Devices: " + String(suspiciousCount) + "\n";
  report += "Anomaly Status: " + (anomalyDetected ? "DETECTED" : "NORMAL") + "\n";
  
  return report;
}
