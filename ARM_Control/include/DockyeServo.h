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
// at a fixed speed (degrees/sec) instead of jumping instantly.
//
// ⚠ call update() every loop() — without it the servo won't move smoothly.
//
// Pulse range: 500–2500µs = 0–180°

class DockyeServo {
public:
    DockyeServo(int pin);

    void begin();
    void setAngle(int targetAngle);        // set target — moves at current speed
    void setSpeed(float degreesPerSec);    // how fast to move (default: 90°/sec)
    void update();                         // MUST be called every loop()
    int  getAngle() const;                 // current physical angle

private:
    Servo         _servo;
    int           _pin;
    float         _currentAngle;
    int           _targetAngle;
    float         _speed;
    unsigned long _lastUpdate;

    static const int MIN_US = 500;
    static const int MAX_US = 2500/1.7;
};
