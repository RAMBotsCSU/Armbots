/*
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
*/
#include "DockyeServo.h"
#include <Arduino.h>

DockyeServo::DockyeServo(int pin)
    : _pin(pin), _currentAngle(0), _targetAngle(0),
      _maxSpeed(90.0f), _accel(45.0f), _currentSpeed(0.0f),
      _lastUpdate(0) {}

void DockyeServo::begin() {
    _servo.attach(_pin, MIN_US, MAX_US);
    _lastUpdate = millis();
    _servo.writeMicroseconds(MIN_US);  // drive to 0° on init
}

void DockyeServo::setAngle(int targetAngle) {
    _targetAngle = constrain(targetAngle, 0, 180);
}

void DockyeServo::setSpeed(float degsPerSec) {
    _maxSpeed = max(1.0f, degsPerSec);
}

void DockyeServo::setAcceleration(float degsPerSec2) {
    _accel = max(1.0f, degsPerSec2);  // minimum 1°/sec² to avoid zero accel
}

void DockyeServo::update() {
    unsigned long now = millis();
    float dt = (now - _lastUpdate) / 1000.0f;
    _lastUpdate = now;

    float remaining = abs(_targetAngle - _currentAngle);

    if (remaining > 0.1f) {
        // Distance needed to decelerate to stop from current speed
        float stopDist = (_currentSpeed * _currentSpeed) / (2.0f * _accel);

        if (stopDist >= remaining) {
            // Close enough to target — start decelerating
            _currentSpeed -= _accel * dt;
            _currentSpeed  = max(_currentSpeed, 1.0f);  // never fully stop until at target
        } else {
            // Still far — accelerate up to max speed
            _currentSpeed += _accel * dt;
            _currentSpeed  = min(_currentSpeed, _maxSpeed);
        }

        float step = _currentSpeed * dt;
        if (_currentAngle < _targetAngle)
            _currentAngle = min((float)_targetAngle, _currentAngle + step);
        else
            _currentAngle = max((float)_targetAngle, _currentAngle - step);
    } else {
        // Arrived at target — reset speed for next move
        _currentAngle = _targetAngle;
        _currentSpeed = 0.0f;
    }

    // Always writeMicroseconds — never write()
    int pulseUs = map((int)_currentAngle, 0, 180, MIN_US, MAX_US);
    _servo.writeMicroseconds(pulseUs);
}

int DockyeServo::getAngle() const {
    return (int)_currentAngle;
}