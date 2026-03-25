#pragma once
#include <ESP32Servo.h>

// ─── StandardServo ────────────────────────────────────────────────────────────
// For the Elbow and Wrist servos.
//
// Speed is controlled by setting a target angle — the servo moves toward it
// at a fixed speed (degrees/sec) instead of jumping instantly.
//
// ⚠ call update() every loop() — without it the servo won't move smoothly.
//
// Usage:
//   StandardServo elbow(48);
//   elbow.begin();
//   elbow.setSpeed(60);     // 60°/sec
//   elbow.setAngle(90);     // sets target — servo starts moving
//   elbow.update();         // call in loop()

class StandardServo {
public:
    StandardServo(int pin);

    void begin();
    void setAngle(int targetAngle);        // set target — moves at current speed
    void setSpeed(float degreesPerSec);    // how fast to move (default: 90°/sec)
    void update();                         // MUST be called every loop()
    int  getAngle() const;                 // current physical angle

private:
    Servo         _servo;
    int           _pin;
    float         _currentAngle;          // actual position (float for smooth steps)
    int           _targetAngle;
    float         _speed;                 // degrees per second
    unsigned long _lastUpdate;            // millis() of last update call

    static const int MIN_US = 500;
    static const int MAX_US = 2500;
};
