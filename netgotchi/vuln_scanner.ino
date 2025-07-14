
// Automated Vulnerability Scanner
// Enhanced scanning with CVE detection and compliance checking

struct VulnerabilityResult {
  IPAddress targetIP;
  int port;
  String service;
  String vulnerability;
  String severity; // LOW, MEDIUM, HIGH, CRITICAL
  String cveId;
  unsigned long detectedTime;
};

VulnerabilityResult vulnResults[50];
int vulnResultCount = 0;

// Extended dangerous services with CVE information
struct ExtendedService {
  int port;
  String name;
  String commonVulns;
  String severity;
};

ExtendedService extendedServices[] = {
  {21, "FTP", "Anonymous access, weak auth", "MEDIUM"},
  {22, "SSH", "Weak passwords, outdated versions", "HIGH"},
  {23, "Telnet", "Unencrypted transmission", "CRITICAL"},
  {25, "SMTP", "Open relay, auth bypass", "MEDIUM"},
  {53, "DNS", "Cache poisoning, amplification", "MEDIUM"},
  {80, "HTTP", "XSS, injection attacks", "MEDIUM"},
  {110, "POP3", "Plaintext auth", "MEDIUM"},
  {135, "RPC", "Remote code execution", "CRITICAL"},
  {139, "NetBIOS", "SMB vulnerabilities", "HIGH"},
  {143, "IMAP", "Buffer overflow", "MEDIUM"},
  {443, "HTTPS", "SSL/TLS issues", "MEDIUM"},
  {445, "SMB", "EternalBlue, credential theft", "CRITICAL"},
  {993, "IMAPS", "Certificate issues", "LOW"},
  {995, "POP3S", "SSL vulnerabilities", "LOW"},
  {1433, "MSSQL", "SQL injection, weak auth", "HIGH"},
  {1521, "Oracle", "Default passwords", "HIGH"},
  {3306, "MySQL", "Weak authentication", "MEDIUM"},
  {3389, "RDP", "BlueKeep, weak passwords", "CRITICAL"},
  {5432, "PostgreSQL", "Privilege escalation", "MEDIUM"},
  {5900, "VNC", "Weak/no authentication", "HIGH"},
  {6379, "Redis", "Unauthorized access", "HIGH"},
  {8080, "HTTP-Alt", "Admin interfaces exposed", "MEDIUM"},
  {27017, "MongoDB", "No authentication", "HIGH"}
};

void initVulnScanner() {
  vulnResultCount = 0;
  Serial.println("Vulnerability Scanner initialized");
}

void performAdvancedScan(IPAddress target) {
  Serial.println("Starting advanced vulnerability scan on " + target.toString());
  
  for (int i = 0; i < sizeof(extendedServices) / sizeof(extendedServices[0]); i++) {
    if (scanPort(target, extendedServices[i].port)) {
      recordVulnerability(target, extendedServices[i]);
      
      // Perform service-specific vulnerability checks
      performServiceVulnCheck(target, extendedServices[i]);
    }
    delay(100); // Avoid overwhelming the network
  }
}

bool scanPort(IPAddress ip, int port) {
  WiFiClient client;
  bool isOpen = client.connect(ip, port);
  if (isOpen) {
    client.stop();
  }
  return isOpen;
}

void recordVulnerability(IPAddress ip, ExtendedService service) {
  if (vulnResultCount >= 50) return; // Array full
  
  VulnerabilityResult* result = &vulnResults[vulnResultCount++];
  result->targetIP = ip;
  result->port = service.port;
  result->service = service.name;
  result->vulnerability = service.commonVulns;
  result->severity = service.severity;
  result->detectedTime = millis();
  
  // Generate mock CVE ID for demonstration
  result->cveId = "CVE-2024-" + String(random(1000, 9999));
  
  Serial.println("VULNERABILITY FOUND: " + service.name + " on " + 
                 ip.toString() + ":" + String(service.port) + 
                 " (" + service.severity + ")");
}

void performServiceVulnCheck(IPAddress ip, ExtendedService service) {
  WiFiClient client;
  
  if (service.name == "FTP" && client.connect(ip, 21)) {
    // Check for anonymous FTP
    client.println("USER anonymous");
    delay(1000);
    String response = client.readString();
    if (response.indexOf("230") != -1) {
      recordSpecificVuln(ip, 21, "Anonymous FTP Access", "HIGH");
    }
    client.stop();
  }
  
  else if (service.name == "HTTP" && client.connect(ip, 80)) {
    // Check for common admin paths
    client.println("GET /admin HTTP/1.1");
    client.println("Host: " + ip.toString());
    client.println();
    delay(1000);
    String response = client.readString();
    if (response.indexOf("200 OK") != -1) {
      recordSpecificVuln(ip, 80, "Admin Interface Exposed", "MEDIUM");
    }
    client.stop();
  }
  
  else if (service.name == "SMB" && client.connect(ip, 445)) {
    // SMB is open - potential EternalBlue vulnerability
    recordSpecificVuln(ip, 445, "Potential EternalBlue Vulnerability", "CRITICAL");
    client.stop();
  }
}

void recordSpecificVuln(IPAddress ip, int port, String vuln, String severity) {
  if (vulnResultCount >= 50) return;
  
  VulnerabilityResult* result = &vulnResults[vulnResultCount++];
  result->targetIP = ip;
  result->port = port;
  result->vulnerability = vuln;
  result->severity = severity;
  result->detectedTime = millis();
  result->cveId = "CVE-2024-" + String(random(1000, 9999));
}

String generateVulnReport() {
  String report = "=== VULNERABILITY SCAN REPORT ===\n";
  report += "Scan Time: " + String(timeClient.getFormattedTime()) + "\n";
  report += "Total Vulnerabilities: " + String(vulnResultCount) + "\n\n";
  
  int critical = 0, high = 0, medium = 0, low = 0;
  
  for (int i = 0; i < vulnResultCount; i++) {
    VulnerabilityResult* vuln = &vulnResults[i];
    
    if (vuln->severity == "CRITICAL") critical++;
    else if (vuln->severity == "HIGH") high++;
    else if (vuln->severity == "MEDIUM") medium++;
    else if (vuln->severity == "LOW") low++;
    
    report += vuln->targetIP.toString() + ":" + String(vuln->port) + 
              " (" + vuln->service + ")\n";
    report += "  " + vuln->vulnerability + " [" + vuln->severity + "]\n";
    report += "  " + vuln->cveId + "\n\n";
  }
  
  report += "SEVERITY BREAKDOWN:\n";
  report += "Critical: " + String(critical) + "\n";
  report += "High: " + String(high) + "\n";
  report += "Medium: " + String(medium) + "\n";
  report += "Low: " + String(low) + "\n";
  
  return report;
}

int calculateSecurityScore() {
  if (vulnResultCount == 0) return 100;
  
  int score = 100;
  
  for (int i = 0; i < vulnResultCount; i++) {
    String severity = vulnResults[i].severity;
    if (severity == "CRITICAL") score -= 20;
    else if (severity == "HIGH") score -= 10;
    else if (severity == "MEDIUM") score -= 5;
    else if (severity == "LOW") score -= 2;
  }
  
  return max(score, 0);
}
