#include "ServoMotor.h"
#include <Arduino.h>

ServoMotor::ServoMotor(int pin, int minUs, int maxUs)
    : _pin(pin), _minUs(minUs), _maxUs(maxUs), _currentAngle(0) {}

void ServoMotor::begin() {
    _servo.attach(_pin, _minUs, _maxUs);
    setAngle(0);
}

void ServoMotor::setAngle(int angle) {
    angle = constrain(angle, 0, 180);
    int pulseUs = map(angle, 0, 180, _minUs, _maxUs);
    _servo.writeMicroseconds(pulseUs);
    _currentAngle = angle;
    Serial.printf("[Servo pin %2d] %3d° → %4d µs\n", _pin, angle, pulseUs);
}

int ServoMotor::getAngle() const {
    return _currentAngle;
}
