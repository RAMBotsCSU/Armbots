#pragma once

// ─── WiFi (AP Mode) ───────────────────────────────────────────────────────────
#define WIFI_SSID     "ArmBot-Control"
#define WIFI_PASSWORD "armbot123"

// ─── OTA (Over The Air) Updates ──────────────────────────────────────────────
// Required when uploading firmware over WiFi instead of USB.
// In platformio.ini set: upload_protocol = espota
//                        upload_port     = 192.168.4.1
#define OTA_PASSWORD  "armbot-ota"

// ─── Servo Pins ───────────────────────────────────────────────────────────────
#define SERVO_ELBOW_PIN     48
#define SERVO_SHOULDER_PIN  38
#define SERVO_WRIST_PIN     47

// ─── Servo Pulse Ranges (microseconds) ───────────────────────────────────────
// Standard servos (Elbow, Wrist)
#define SERVO_STANDARD_MIN_US  500
#define SERVO_STANDARD_MAX_US  2500

// Dockye servo (Shoulder) — uses same 500–2500µs range but MUST use
// writeMicroseconds(), NOT write(). Standard writeDegrees() is inaccurate
// for this servo model.
#define DOCKYE_MIN_US  500
#define DOCKYE_MAX_US  2500

// ─── Stepper: Base (A4988 driver) ────────────────────────────────────────────
#define BASE_ENABLE_PIN  4   // ⚠ SHARED with Gripper stepper — both enable together
#define BASE_STEP_PIN    1
#define BASE_DIR_PIN     2
#define BASE_MS1_PIN     15
#define BASE_MS2_PIN     16
#define BASE_MS3_PIN     17

// ─── Stepper: Gripper (A4988 driver) ─────────────────────────────────────────
#define GRIPPER_ENABLE_PIN  4   // ⚠ SHARED with Base stepper — both enable together
#define GRIPPER_STEP_PIN    21
#define GRIPPER_DIR_PIN     5
#define GRIPPER_MS1_PIN     9
#define GRIPPER_MS2_PIN     10
#define GRIPPER_MS3_PIN     11

// ─── Stepper Motion Settings ─────────────────────────────────────────────────
#define STEPPER_MAX_SPEED    1000.0f   // steps per second
#define STEPPER_ACCELERATION  500.0f  // steps per second²
