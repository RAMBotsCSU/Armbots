#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "config.h"
#include "ServoMotor.h"
#include "StepperMotor.h"

// ─── Motors ───────────────────────────────────────────────────────────────────
//                           pin                  min µs                 max µs
ServoMotor elbowServo   (SERVO_ELBOW_PIN,    SERVO_STANDARD_MIN_US, SERVO_STANDARD_MAX_US);
ServoMotor shoulderServo(SERVO_SHOULDER_PIN, DOCKYE_MIN_US,         DOCKYE_MAX_US);
ServoMotor wristServo   (SERVO_WRIST_PIN,    SERVO_STANDARD_MIN_US, SERVO_STANDARD_MAX_US);

//                            enable              step              dir             MS1            MS2            MS3
StepperMotor baseStepper   (BASE_ENABLE_PIN,    BASE_STEP_PIN,    BASE_DIR_PIN,    BASE_MS1_PIN,    BASE_MS2_PIN,    BASE_MS3_PIN);
StepperMotor gripperStepper(GRIPPER_ENABLE_PIN, GRIPPER_STEP_PIN, GRIPPER_DIR_PIN, GRIPPER_MS1_PIN, GRIPPER_MS2_PIN, GRIPPER_MS3_PIN);

// ─── Web Server ───────────────────────────────────────────────────────────────
WebServer server(80);

void handleRoot() {
    server.send(200, "text/plain", "ArmBot online. Web UI coming soon.");
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n─── ArmBot Booting ───────────────────────────────");

    // ── Init Motors ──────────────────────────────────────────────────────────
    Serial.println("[Motors] Initializing servos...");
    elbowServo.begin();
    shoulderServo.begin();
    wristServo.begin();

    Serial.println("[Motors] Initializing steppers...");
    baseStepper.begin();
    gripperStepper.begin();

    // ── Start WiFi AP ────────────────────────────────────────────────────────
    Serial.println("[WiFi] Starting Access Point...");
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("[WiFi] SSID: %s\n", WIFI_SSID);
    Serial.printf("[WiFi] IP:   http://%s\n", WiFi.softAPIP().toString().c_str());

    // ── Register Routes ──────────────────────────────────────────────────────
    server.on("/", handleRoot);
    server.begin();

    Serial.println("[Server] Running.");
    Serial.println("─────────────────────────────────────────────────\n");
}

void loop() {
    // Handle any incoming web requests
    server.handleClient();

    // Steppers MUST be called every loop — they move one step at a time
    baseStepper.run();
    gripperStepper.run();
}
