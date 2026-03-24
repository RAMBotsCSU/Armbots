#include "DockyeServo.h"
#include <Arduino.h>

DockyeServo::DockyeServo(int pin)
    : _pin(pin), _currentAngle(0) {}

void DockyeServo::begin() {
    _servo.attach(_pin, MIN_US, MAX_US);
    setAngle(0);
}

void DockyeServo::setAngle(int angle) {
    angle = constrain(angle, 0, 180);
    int pulseUs = map(angle, 0, 180, MIN_US, MAX_US);
    _servo.writeMicroseconds(pulseUs);  // always writeMicroseconds — never write()
    _currentAngle = angle;
    Serial.printf("[DockyeServo  pin %2d] %3d° → %4d µs\n", _pin, angle, pulseUs);
}

int DockyeServo::getAngle() const {
    return _currentAngle;
}
