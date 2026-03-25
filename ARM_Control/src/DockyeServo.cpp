#include "DockyeServo.h"
#include <Arduino.h>

DockyeServo::DockyeServo(int pin)
    : _pin(pin), _currentAngle(0), _targetAngle(0),
      _speed(90.0f), _lastUpdate(0) {}

void DockyeServo::begin() {
    _servo.attach(_pin, MIN_US, MAX_US);
    _lastUpdate = millis();
    setAngle(0);
}

void DockyeServo::setAngle(int targetAngle) {
    _targetAngle = constrain(targetAngle, 0, 180);
}

void DockyeServo::setSpeed(float degreesPerSec) {
    _speed = max(1.0f, degreesPerSec);
}

void DockyeServo::update() {
    if ((int)_currentAngle == _targetAngle) return;

    unsigned long now = millis();
    float dt = (now - _lastUpdate) / 1000.0f;
    _lastUpdate = now;

    float step = _speed * dt;

    if (_currentAngle < _targetAngle)
        _currentAngle = min((float)_targetAngle, _currentAngle + step);
    else
        _currentAngle = max((float)_targetAngle, _currentAngle - step);

    int pulseUs = map((int)_currentAngle, 0, 180, MIN_US, MAX_US);
    _servo.writeMicroseconds(pulseUs);  // always writeMicroseconds — never write()
}

int DockyeServo::getAngle() const {
    return (int)_currentAngle;
}
