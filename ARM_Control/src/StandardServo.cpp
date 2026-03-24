#include "StandardServo.h"
#include <Arduino.h>

StandardServo::StandardServo(int pin)
    : _pin(pin), _currentAngle(0) {}

void StandardServo::begin() {
    _servo.attach(_pin, MIN_US, MAX_US);
    setAngle(0);
}

void StandardServo::setAngle(int angle) {
    angle = constrain(angle, 0, 180);
    int pulseUs = map(angle, 0, 180, MIN_US, MAX_US);
    _servo.writeMicroseconds(pulseUs);
    _currentAngle = angle;
    Serial.printf("[StandardServo pin %2d] %3d° → %4d µs\n", _pin, angle, pulseUs);
}

int StandardServo::getAngle() const {
    return _currentAngle;
}
