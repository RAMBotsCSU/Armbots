#include "StandardServo.h"
#include <Arduino.h>

StandardServo::StandardServo(int pin)
    : _pin(pin), _currentAngle(0), _targetAngle(0),
      _speed(90.0f), _lastUpdate(0) {}

// Converts pulse width in microseconds to a 14-bit duty cycle value.
// Formula: duty = pulseUs / 20000µs (50Hz period) * 16383 (2^14 - 1)
// ⚠ 14-bit is the maximum resolution on ESP32-S3 — 16-bit causes ledcSetup errors
uint32_t StandardServo::angleToDuty(int pulseUs) {
    return (uint32_t)((float)pulseUs / 20000.0f * 16383.0f);
}

void StandardServo::begin() {
    // attachPin replaces servo.attach() — explicitly sets 50Hz and 14-bit resolution
    // ⚠ must be called before WiFi.softAP() — WiFi steals LEDC channels if it starts first
    _pwm.attachPin(_pin, 50, 14);
    _lastUpdate = millis();
    _pwm.write(angleToDuty(MIN_US));  // drive to 0° immediately on init
}

void StandardServo::setAngle(int targetAngle) {
    _targetAngle = constrain(targetAngle, 0, 180);
}

void StandardServo::setSpeed(float degreesPerSec) {
    _speed = max(1.0f, degreesPerSec);  // minimum 1°/sec to avoid divide-by-zero
}

void StandardServo::update() {
    unsigned long now = millis();
    float dt = (now - _lastUpdate) / 1000.0f;  // seconds since last update
    _lastUpdate = now;

    // Interpolate toward target at _speed degrees/sec
    if ((int)_currentAngle != _targetAngle) {
        float step = _speed * dt;
        if (_currentAngle < _targetAngle)
            _currentAngle = min((float)_targetAngle, _currentAngle + step);
        else
            _currentAngle = max((float)_targetAngle, _currentAngle - step);
    }

    // Always write PWM every loop — servo needs continuous signal to hold position
    // Previously only wrote when moving, causing servo to go limp at target angle
    int pulseUs = map((int)_currentAngle, 0, 180, MIN_US, MAX_US);
    _pwm.write(angleToDuty(pulseUs));
}

int StandardServo::getAngle() const {
    return (int)_currentAngle;
}