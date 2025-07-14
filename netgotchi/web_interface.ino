
// Mobile-optimized web interface for netgotchi

String getMobileOptimizedCSS() {
  return R"(
<style>
* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
}

body {
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
  background: #000;
  color: #00ff00;
  min-height: 100vh;
  padding: 10px;
  font-size: 16px;
  line-height: 1.4;
}

.container {
  max-width: 100%;
  margin: 0 auto;
  padding: 20px;
}

.header {
  text-align: center;
  margin-bottom: 30px;
  border-bottom: 2px solid #00ff00;
  padding-bottom: 20px;
}

.header h1 {
  font-size: 2rem;
  margin-bottom: 10px;
  text-shadow: 0 0 10px #00ff00;
}

.status-grid {
  display: grid;
  grid-template-columns: 1fr;
  gap: 15px;
  margin-bottom: 30px;
}

@media (min-width: 768px) {
  .status-grid {
    grid-template-columns: repeat(2, 1fr);
  }
}

.status-card {
  background: rgba(0, 255, 0, 0.1);
  border: 1px solid #00ff00;
  border-radius: 8px;
  padding: 15px;
  backdrop-filter: blur(5px);
}

.status-card h3 {
  margin-bottom: 10px;
  color: #00ff88;
  font-size: 1.1rem;
}

.controls {
  display: grid;
  grid-template-columns: 1fr;
  gap: 15px;
  margin-bottom: 30px;
}

@media (min-width: 480px) {
  .controls {
    grid-template-columns: repeat(2, 1fr);
  }
}

@media (min-width: 768px) {
  .controls {
    grid-template-columns: repeat(3, 1fr);
  }
}

.btn {
  background: linear-gradient(45deg, #003300, #006600);
  border: 2px solid #00ff00;
  color: #00ff00;
  padding: 15px 20px;
  border-radius: 8px;
  font-size: 1rem;
  font-weight: bold;
  cursor: pointer;
  transition: all 0.3s ease;
  text-decoration: none;
  display: block;
  text-align: center;
  min-height: 60px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.btn:hover, .btn:active {
  background: linear-gradient(45deg, #006600, #00aa00);
  box-shadow: 0 0 15px rgba(0, 255, 0, 0.5);
  transform: translateY(-2px);
}

.btn:active {
  transform: translateY(0);
}

.btn.danger {
  border-color: #ff4444;
  color: #ff4444;
  background: linear-gradient(45deg, #330000, #660000);
}

.btn.danger:hover, .btn.danger:active {
  background: linear-gradient(45deg, #660000, #aa0000);
  box-shadow: 0 0 15px rgba(255, 68, 68, 0.5);
}

.display-section {
  background: rgba(0, 255, 0, 0.05);
  border: 1px solid #00ff00;
  border-radius: 8px;
  padding: 20px;
  margin-bottom: 20px;
  text-align: center;
}

.pixel-display {
  background: #000;
  border: 2px solid #00ff00;
  border-radius: 4px;
  padding: 10px;
  margin: 20px auto;
  max-width: 100%;
  overflow-x: auto;
  font-family: monospace;
  font-size: 12px;
  line-height: 1;
}

.log-output {
  background: rgba(0, 0, 0, 0.8);
  border: 1px solid #00ff00;
  border-radius: 4px;
  padding: 15px;
  font-family: monospace;
  font-size: 14px;
  max-height: 300px;
  overflow-y: auto;
  white-space: pre-wrap;
  word-break: break-all;
}

.alert {
  padding: 15px;
  border-radius: 8px;
  margin-bottom: 20px;
  font-weight: bold;
  text-align: center;
}

.alert.warning {
  background: rgba(255, 165, 0, 0.2);
  border: 1px solid #ffa500;
  color: #ffa500;
}

.alert.danger {
  background: rgba(255, 68, 68, 0.2);
  border: 1px solid #ff4444;
  color: #ff4444;
  animation: pulse 2s infinite;
}

@keyframes pulse {
  0%, 100% { opacity: 1; }
  50% { opacity: 0.7; }
}

.footer {
  text-align: center;
  margin-top: 40px;
  padding-top: 20px;
  border-top: 1px solid #00ff00;
  opacity: 0.7;
  font-size: 0.9rem;
}

/* Touch-friendly improvements */
@media (hover: none) and (pointer: coarse) {
  .btn {
    min-height: 50px;
    font-size: 1.1rem;
  }
  
  .controls {
    gap: 20px;
  }
  
  .status-card {
    padding: 20px;
  }
}

/* Landscape phone optimization */
@media screen and (max-height: 500px) and (orientation: landscape) {
  .header h1 {
    font-size: 1.5rem;
  }
  
  .container {
    padding: 10px;
  }
  
  .status-grid {
    grid-template-columns: repeat(3, 1fr);
  }
}

/* Dark mode enhancements */
@media (prefers-color-scheme: dark) {
  body {
    background: #000;
  }
}

/* High contrast mode */
@media (prefers-contrast: high) {
  .btn {
    border-width: 3px;
  }
  
  .status-card {
    border-width: 2px;
  }
}
</style>
)";
}

String getMobileOptimizedHTML() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>";
  html += "<meta name='apple-mobile-web-app-capable' content='yes'>";
  html += "<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent'>";
  html += "<meta name='theme-color' content='#000000'>";
  html += "<title>Netgotchi Mobile Control</title>";
  html += getMobileOptimizedCSS();
  html += "</head><body>";
  
  html += "<div class='container'>";
  html += "<div class='header'>";
  html += "<h1>🛸 Netgotchi</h1>";
  html += "<div>Mobile Control Panel</div>";
  html += "</div>";
  
  // Alert section for threats
  if (honeypotTriggered || evilTwinDetected) {
    html += "<div class='alert danger'>";
    html += "⚠️ SECURITY ALERT DETECTED ⚠️";
    html += "</div>";
  }
  
  if (powerStatus.lowPowerMode) {
    html += "<div class='alert warning'>";
    html += "🔋 Low Power Mode Active";
    html += "</div>";
  }
  
  // Status grid
  html += "<div class='status-grid'>";
  
  html += "<div class='status-card'>";
  html += "<h3>📡 Network Status</h3>";
  html += "<div>WiFi: " + String(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected") + "</div>";
  html += "<div>SSID: " + WiFi.SSID() + "</div>";
  html += "<div>IP: " + WiFi.localIP().toString() + "</div>";
  html += "</div>";
  
  html += "<div class='status-card'>";
  html += "<h3>🔋 Power Status</h3>";
  html += "<div>Battery: " + String(powerStatus.batteryPercentage) + "%</div>";
  html += "<div>Voltage: " + String(powerStatus.batteryVoltage, 2) + "V</div>";
  html += "<div>Mode: " + String(powerStatus.lowPowerMode ? "Low Power" : "Normal") + "</div>";
  html += "</div>";
  
  html += "<div class='status-card'>";
  html += "<h3>🎯 Scan Results</h3>";
  html += "<div>Hosts Found: " + String(ipnum) + "</div>";
  html += "<div>Vulnerabilities: " + String(vulnerabilitiesFound) + "</div>";
  html += "<div>Status: " + String(startScan ? "Scanning" : "Idle") + "</div>";
  html += "</div>";
  
  html += "<div class='status-card'>";
  html += "<h3>🍯 Honeypot</h3>";
  html += "<div>Status: " + String(honeypotTriggered ? "BREACHED" : "OK") + "</div>";
  html += "<div>Evil Twin: " + String(evilTwinDetected ? "DETECTED" : "Clear") + "</div>";
  html += "</div>";
  
  html += "</div>"; // End status grid
  
  // Control buttons
  html += "<div class='controls'>";
  html += "<a href='/scan' class='btn'>🔍 Start Scan</a>";
  html += "<a href='/stop' class='btn'>⏹️ Stop Scan</a>";
  html += "<a href='/ping' class='btn'>📡 Network Test</a>";
  html += "<a href='/reset_honeypot' class='btn'>🍯 Reset Honeypot</a>";
  html += "<a href='/settings' class='btn'>⚙️ Settings</a>";
  html += "<a href='/power' class='btn'>🔋 Power Options</a>";
  html += "<a href='/logs' class='btn'>📋 View Logs</a>";
  html += "<a href='/restart' class='btn danger'>🔄 Restart</a>";
  html += "</div>";
  
  // Display section
  html += "<div class='display-section'>";
  html += "<h3>📺 Current Display</h3>";
  html += "<div class='pixel-display' id='display'>";
  html += "Loading display...";
  html += "</div>";
  html += "<div class='controls'>";
  html += "<a href='/next_screen' class='btn'>⏭️ Next Screen</a>";
  html += "<a href='/toggle_animation' class='btn'>🎬 Toggle Animation</a>";
  html += "</div>";
  html += "</div>";
  
  html += "<div class='footer'>";
  html += "Netgotchi v" + String(VERSION) + " | Uptime: " + String(seconds) + "s";
  html += "</div>";
  
  html += "</div>"; // End container
  
  // Auto-refresh script
  html += "<script>";
  html += "setInterval(function(){";
  html += "  fetch('/api/status').then(r=>r.json()).then(data=>{";
  html += "    if(data.display) document.getElementById('display').innerHTML = data.display;";
  html += "  }).catch(e=>console.log(e));";
  html += "}, 5000);"; // Refresh every 5 seconds
  html += "</script>";
  
  html += "</body></html>";
  return html;
}

void setupMobileWebInterface() {
  // Main mobile interface
  server.on("/", []() {
    server.send(200, "text/html", getMobileOptimizedHTML());
  });
  
  // API endpoint for status updates
  server.on("/api/status", []() {
    String json = "{";
    json += "\"wifi_connected\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
    json += "\"battery\":" + String(powerStatus.batteryPercentage) + ",";
    json += "\"hosts\":" + String(ipnum) + ",";
    json += "\"vulnerabilities\":" + String(vulnerabilitiesFound) + ",";
    json += "\"honeypot\":" + String(honeypotTriggered ? "true" : "false") + ",";
    json += "\"scanning\":" + String(startScan ? "true" : "false") + ",";
    json += "\"display\":\"" + getPixelMatrix() + "\"";
    json += "}";
    server.send(200, "application/json", json);
  });
  
  // Touch-friendly control endpoints
  server.on("/scan", []() {
    startScan = true;
    server.sendHeader("Location", "/");
    server.send(302);
  });
  
  server.on("/stop", []() {
    startScan = false;
    server.sendHeader("Location", "/");
    server.send(302);
  });
  
  server.on("/next_screen", []() {
    nextScreen();
    server.sendHeader("Location", "/");
    server.send(302);
  });
  
  server.on("/toggle_animation", []() {
    animation++;
    if (animation > max_anim) animation = 0;
    server.sendHeader("Location", "/");
    server.send(302);
  });
  
  server.on("/reset_honeypot", []() {
    honeypotTriggered = false;
    evilTwinDetected = false;
    server.sendHeader("Location", "/");
    server.send(302);
  });
  
  server.on("/restart", []() {
    server.send(200, "text/html", 
      "<html><body style='background:#000;color:#00ff00;text-align:center;font-family:monospace;padding:50px;'>"
      "<h1>Restarting Netgotchi...</h1><p>Please wait...</p></body></html>");
    delay(2000);
    ESP.restart();
  });
}
