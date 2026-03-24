#pragma once
#include <ESP32Servo.h>

// ─── StandardServo ────────────────────────────────────────────────────────────
// For the Elbow and Wrist servos.
// Uses full 500–2500µs pulse range via writeMicroseconds() for accuracy.
//
// Usage:
//   StandardServo elbow(48);
//   elbow.begin();
//   elbow.setAngle(90);

class StandardServo {
public:
    StandardServo(int pin);

    void begin();             // attach and move to 0°
    void setAngle(int angle); // move to angle (0–180°)
    int  getAngle() const;    // return last commanded angle

private:
    Servo _servo;
    int   _pin;
    int   _currentAngle;

    static const int MIN_US = 500;
    static const int MAX_US = 2500;
};
