#include "StandardServo.h"
#include <Arduino.h>

StandardServo::StandardServo(int pin)
    : _pin(pin), _currentAngle(0), _targetAngle(0),
      _speed(90.0f), _lastUpdate(0) {}

void StandardServo::begin() {
    _servo.attach(_pin, MIN_US, MAX_US);
    _lastUpdate = millis();
    setAngle(0);
}

void StandardServo::setAngle(int targetAngle) {
    _targetAngle = constrain(targetAngle, 0, 180);
}

void StandardServo::setSpeed(float degreesPerSec) {
    _speed = max(1.0f, degreesPerSec);  // minimum 1°/sec to avoid divide-by-zero
}

void StandardServo::update() {
    if ((int)_currentAngle == _targetAngle) return;

    unsigned long now = millis();
    float dt = (now - _lastUpdate) / 1000.0f;  // seconds since last update
    _lastUpdate = now;

    float step = _speed * dt;

    if (_currentAngle < _targetAngle)
        _currentAngle = min((float)_targetAngle, _currentAngle + step);
    else
        _currentAngle = max((float)_targetAngle, _currentAngle - step);

    int pulseUs = map((int)_currentAngle, 0, 180, MIN_US, MAX_US);
    _servo.writeMicroseconds(pulseUs);
}

int StandardServo::getAngle() const {
    return (int)_currentAngle;
}
