#pragma once
#include <ESP32Servo.h>

// ─── DockyeServo ──────────────────────────────────────────────────────────────
// For the Shoulder servo (Dockye special model).
//
// ⚠ This servo MUST be controlled via pulse width (writeMicroseconds).
//   Never use write() or writeDegrees() — the angle mapping is inaccurate
//   for this model and will cause wrong positions.
//
// Speed is controlled by setting a target angle — the servo moves toward it
// with acceleration/deceleration (trapezoidal profile).
//
// ⚠ call update() every loop() — without it the servo won't move smoothly.
//
// Pulse range: 500–2500µs = 0–180°

class DockyeServo {
public:
    DockyeServo(int pin);

    void begin();
    void setAngle(int targetAngle);          // set target — moves at current speed
    void setSpeed(float degreesPerSec);      // max speed (default: 90°/sec)
    void setAcceleration(float degsPerSec2); // ramp rate (default: 45°/sec²)
    void update();                           // MUST be called every loop()
    int  getAngle() const;                   // current physical angle

private:
    Servo         _servo;
    int           _pin;
    float         _currentAngle;
    int           _targetAngle;
    float         _maxSpeed;
    float         _accel;
    float         _currentSpeed;
    unsigned long _lastUpdate;

    static const int MIN_US = 500;
    static const int MAX_US = 2500/1.7;
};
