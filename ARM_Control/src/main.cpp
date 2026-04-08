#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "config.h"
#include "ArmController.h"
#include "ui.h"
#include "BallGrabRoutine.h"  

// ─── Arm ──────────────────────────────────────────────────────────────────────
ArmController arm;
BallGrabRoutine ballGrabRoutine(arm);

// ─── Web Server + WebSocket ───────────────────────────────────────────────────
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                      AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("[WS] Client #%u connected\n", client->id());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("[WS] Client #%u disconnected\n", client->id());
            break;
            //Receives user input from the UI
        case WS_EVT_DATA: {
            String msg = String((char*)data).substring(0, len);
            Serial.printf("[WS] Received: %s\n", msg.c_str());

            int spaceIdx = msg.indexOf(' ');
            String joint = (spaceIdx > 0) ? msg.substring(0, spaceIdx) : msg;
            int value    = (spaceIdx > 0) ? msg.substring(spaceIdx + 1).toInt() : 0;
            joint.toLowerCase();

            if      (joint == "elbow")    arm.setElbow(value);
            else if (joint == "shoulder") arm.setShoulder(value);
            else if (joint == "wrist")    arm.setWrist(value);
            else if (joint == "base")         { arm.moveBase(value);             }
            else if (joint == "gripper")      { arm.moveGripper(value);          }
            else if (joint == "baseangle")    { arm.moveBaseToAngle(value);    }
            else if (joint == "gripperangle") { arm.moveGripperToAngle(value); }
            else if (joint == "ballgrab") { ballGrabRoutine.run(); }
            else if (joint == "home")     {
                arm.home();
                ws.textAll("reset");
            }
            else Serial.println("[WS] Unknown command.");
            break;
        }
        default:
            break;
    }
}

// ─── Serial Test Commands ─────────────────────────────────────────────────────
// Used to test each joint individually before the web UI is built.
// Remove this function and its loop() call once the web UI is working.
//
// Commands (type in Serial monitor, set line ending to "Newline"):
//   elbow    <0-180>   e.g.  elbow 90
//   shoulder <0-180>   e.g.  shoulder 45
//   wrist    <0-180>   e.g.  wrist 120
//   base     <steps>   e.g.  base 500
//   gripper  <steps>   e.g.  gripper 200
//   home               returns all joints to zero

void handleSerial() {
    if (!Serial.available()) return;

    String input = Serial.readStringUntil('\n');
    input.trim();

    int spaceIdx  = input.indexOf(' ');
    String joint  = (spaceIdx > 0) ? input.substring(0, spaceIdx) : input;
    int    value  = (spaceIdx > 0) ? input.substring(spaceIdx + 1).toInt() : 0;

    joint.toLowerCase();

    if      (joint == "elbow")    { arm.setElbow(value);    }
    else if (joint == "shoulder") { arm.setShoulder(value); }
    else if (joint == "wrist")    { arm.setWrist(value);    }
    else if (joint == "base")     { arm.moveBase(value);    }
    else if (joint == "gripper")  { arm.moveGripper(value); }
    else if (joint == "baseangle")    { arm.moveBaseToAngle(value);    }
    else if (joint == "gripperangle") { arm.moveGripperToAngle(value); }
    else if (joint == "home")     { arm.home();             }
    else {
        Serial.println("[Serial] Unknown command. Try: elbow 90 | shoulder 45 | wrist 120 | base 500 | gripper 200 | home");
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    Serial.println("\n─── ArmBot Booting ───────────────────────────────");
    

    arm.begin();

    Serial.println("[WiFi] Starting Access Point...");
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("[WiFi] SSID: %s\n", WIFI_SSID);
    Serial.printf("[WiFi] IP:   http://%s\n", WiFi.softAPIP().toString().c_str());

    ws.onEvent(onWebSocketEvent);
    server.addHandler(&ws);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
        //req->send(200, "text/plain", "ArmBot online. Web UI coming soon.");
        req->send(200, "text/html", INDEX_HTML);
    });
    server.begin();

    Serial.println("[Server] Running.");
    Serial.println("─────────────────────────────────────────────────\n");
}
    
void loop() {
    ws.cleanupClients();
    arm.run();
    handleSerial();
}
