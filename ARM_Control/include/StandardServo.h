#pragma once
#include <ESP32PWM.h>  // replaced ESP32Servo — direct LEDC control avoids channel conflicts on ESP32-S3

// ─── StandardServo ────────────────────────────────────────────────────────────
// For the Elbow, Shoulder, and Wrist servos.
//
// ⚠ Previously used ESP32Servo which silently dropped the 3rd servo's PWM
//   signal on ESP32-S3 due to LEDC channel conflicts. Switched to ESP32PWM
//   with explicit attachPin(pin, 50Hz, 14-bit) — 14-bit is the ESP32-S3 max.
//
// ⚠ PWM duty cycle is calculated manually:
//   duty = pulseUs / 20000 * 16383  (16383 = 2^14 - 1)
//
// Speed is controlled by setting a target angle — the servo moves toward it
// at a fixed speed (degrees/sec) instead of jumping instantly.
//
// ⚠ call update() every loop() — without it the servo won't move smoothly.
//
// Pulse range: 500–2500µs = 0–180°

class StandardServo {
public:
    StandardServo(int pin);

    void begin();
    void setAngle(int targetAngle);        // set target — moves at current speed
    void setSpeed(float degreesPerSec);    // how fast to move (default: 90°/sec)
    void update();                         // MUST be called every loop()
    int  getAngle() const;                 // current physical angle

private:
    ESP32PWM      _pwm;                   // replaced Servo — direct ESP32 PWM control
    int           _pin;
    float         _currentAngle;          // actual position (float for smooth steps)
    int           _targetAngle;
    float         _speed;                 // degrees per second
    unsigned long _lastUpdate;            // millis() of last update call

    static const int MIN_US = 500;
    static const int MAX_US = 2500;

    uint32_t angleToDuty(int pulseUs);    // converts µs to 14-bit duty cycle
};