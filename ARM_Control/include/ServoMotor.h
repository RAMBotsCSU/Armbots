#pragma once
#include <ESP32Servo.h>

// ─── ServoMotor ───────────────────────────────────────────────────────────────
// Wraps a single servo. Works for both standard servos AND the Dockye servo.
// Always controls angle via pulse width (microseconds) for accuracy.
//
// Usage:
//   ServoMotor myServo(PIN, MIN_US, MAX_US);
//   myServo.begin();
//   myServo.setAngle(90);

class ServoMotor {
public:
    ServoMotor(int pin, int minUs, int maxUs);

    void begin();             // attach servo and move to 0°
    void setAngle(int angle); // move to angle (0–180°)
    int  getAngle() const;    // return last commanded angle

private:
    Servo _servo;
    int   _pin;
    int   _minUs;
    int   _maxUs;
    int   _currentAngle;
};
