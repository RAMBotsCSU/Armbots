#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Servo.h>


// ─── WiFi Credentials ─────────────────────────────────────────────────
const char* ssid     = "yourssid";
const char* password = "yourpasswd";

// ─── Servo Setup ────────────────────────────
Servo dockyeServo;
const int SERVO_PIN  = 9;
const int MIN_PULSE  = 500;
const int MAX_PULSE  = 1250;   // 2500/2

int currentAngle = 90;

// ─── Web Server ───────────────────────────────────────────────────────
WiFiServer server(80);

// ─── Servo Helper ─────────────────────────────────────────────────────
void setServoAngle(int angle) {
  angle = constrain(angle, 0, 180);
  int pulseWidth = map(angle, 0, 180, MIN_PULSE, MAX_PULSE);
  dockyeServo.writeMicroseconds(pulseWidth);
  currentAngle = angle;
  Serial.print("Servo | Angle: ");
  Serial.print(angle);
  Serial.print("° | Pulse: ");
  Serial.print(pulseWidth);
  Serial.println("us");
}

void setup() {
  Serial.begin(115200);

  // Servo
  dockyeServo.attach(SERVO_PIN, MIN_PULSE, MAX_PULSE);
  setServoAngle(0);  // start at zero
  Serial.println("Servo attached. Starting at 90°");

  // WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("Open this in your browser: http://");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // ── Send HTML page ──────────────────────────────────────
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE html><html><head>");
            client.println("<meta charset='UTF-8'/>");
            client.println("<meta name='viewport' content='width=device-width,initial-scale=1'/>");
            client.println("<title>Servo Control</title>");
            client.println("<style>");
            client.println("  body { font-family: monospace; background:#0d1117; color:#c9d1d9;");
            client.println("         display:flex; flex-direction:column; align-items:center;");
            client.println("         justify-content:center; height:100vh; margin:0; }");
            client.println("  h2 { color:#58a6ff; letter-spacing:0.1em; margin-bottom:32px; }");
            client.println("  .card { background:#161b22; border:1px solid #30363d;");
            client.println("          border-radius:12px; padding:32px 36px; min-width:300px; text-align:center; }");
            client.println("  .angle { font-size:3rem; font-weight:bold; color:#58a6ff; margin:16px 0 8px; }");
            client.println("  input[type=range] { width:100%; accent-color:#58a6ff;");
            client.println("                      height:6px; cursor:pointer; margin:16px 0; }");
            client.println("  .labels { display:flex; justify-content:space-between;");
            client.println("            font-size:0.75rem; color:#8b949e; }");
            client.println("  .presets { display:flex; gap:8px; margin-top:20px; }");
            client.println("  button { flex:1; padding:10px 0; background:#21262d;");
            client.println("           border:1px solid #30363d; border-radius:8px;");
            client.println("           color:#c9d1d9; font-family:monospace; cursor:pointer; }");
            client.println("  button:hover { border-color:#58a6ff; color:#58a6ff; }");
            client.println("</style></head><body>");
            client.println("<h2>SERVO CONTROL</h2>");
            client.println("<div class='card'>");
            client.print("  <div class='angle' id='val'>");
            client.print(currentAngle);
            client.println("°</div>");
            client.print("  <input type='range' id='slider' min='0' max='180' value='");
            client.print(currentAngle);
            client.println("' oninput=\"document.getElementById('val').innerText=this.value+'°'\"");
            client.println("           onchange=\"fetch('/S?a='+this.value)\"/>");
            client.println("  <div class='labels'><span>0°</span><span>90°</span><span>180°</span></div>");
            client.println("  <div class='presets'>");
            client.println("    <button onclick=\"setAngle(0)\">0°</button>");
            client.println("    <button onclick=\"setAngle(90)\">90°</button>");
            client.println("    <button onclick=\"setAngle(180)\">180°</button>");
            client.println("  </div>");
            client.println("</div>");
            client.println("<script>");
            client.println("  function setAngle(a) {");
            client.println("    document.getElementById('slider').value = a;");
            client.println("    document.getElementById('val').innerText = a + '°';");
            client.println("    fetch('/S?a=' + a);");
            client.println("  }");
            client.println("</script>");
            client.println("</body></html>");
            break;

          } else {
            currentLine = "";
          }

        } else if (c != '\r') {
          currentLine += c;

          // ── Parse angle from GET /S?a=XX ────────────────────────
          if (currentLine.startsWith("GET /S?a=")) {
            int spaceIdx = currentLine.indexOf(' ', 9);
            String angleStr = (spaceIdx > 0)
                              ? currentLine.substring(9, spaceIdx)
                              : currentLine.substring(9);

            // Wait until full line received (ends with space + HTTP)
            if (currentLine.endsWith("HTTP/1.1") || spaceIdx > 0) {
              int angle = angleStr.toInt();
              setServoAngle(angle);

              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/plain");
              client.println("Connection: close");
              client.println();
              client.print("OK:");
              client.println(angle);
              break;
            }
          }
        }
      }
    }

    client.stop();
    Serial.println("Client Disconnected.");
  }
}