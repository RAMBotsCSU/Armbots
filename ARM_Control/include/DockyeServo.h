#pragma once
#include <ESP32Servo.h>

// ─── DockyeServo ──────────────────────────────────────────────────────────────
// For the Shoulder servo (Dockye special model).
//
// ⚠ This servo MUST be controlled via pulse width (writeMicroseconds).
//   Never use write() or writeDegrees() — the angle mapping is inaccurate
//   for this model and will cause wrong positions.
//
// Pulse range: 500–2500µs = 0–180°
//
// Usage:
//   DockyeServo shoulder(38);
//   shoulder.begin();
//   shoulder.setAngle(90);

class DockyeServo {
public:
    DockyeServo(int pin);

    void begin();             // attach and move to 0°
    void setAngle(int angle); // move to angle (0–180°) via pulse width only
    int  getAngle() const;    // return last commanded angle

private:
    Servo _servo;
    int   _pin;
    int   _currentAngle;

    static const int MIN_US = 500;
    static const int MAX_US = 2500/1.7;
};
