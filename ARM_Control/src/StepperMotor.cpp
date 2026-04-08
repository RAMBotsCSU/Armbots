#include "StepperMotor.h"
#include "config.h"
#include <Arduino.h>

// --- Motor characteristics ---
static constexpr float STEPS_PER_REV = 200.0f;  // NEMA 17 (1.8° per step)
static constexpr float GEAR_RATIO    = 10.0f;    // 10:1 gear ratio


StepperMotor::StepperMotor(int enablePin, int stepPin, int dirPin,
                            int ms1Pin, int ms2Pin, int ms3Pin)
    : _stepper(AccelStepper::DRIVER, stepPin, dirPin),
      _enablePin(enablePin),
      _ms1Pin(ms1Pin), _ms2Pin(ms2Pin), _ms3Pin(ms3Pin) {}

void StepperMotor::begin() {
    pinMode(_enablePin, OUTPUT);
    pinMode(_ms1Pin,    OUTPUT);
    pinMode(_ms2Pin,    OUTPUT);
    pinMode(_ms3Pin,    OUTPUT);

    // A4988 ENABLE pin is active LOW — pull LOW to activate the driver
    digitalWrite(_enablePin, LOW);

    // Microstepping: Full step (MS1=LOW, MS2=LOW, MS3=LOW)
    // Change these to set 1/2, 1/4, 1/8, or 1/16 step mode
    digitalWrite(_ms1Pin, LOW);
    digitalWrite(_ms2Pin, LOW);
    digitalWrite(_ms3Pin, LOW);

    _stepper.setMaxSpeed(STEPPER_MAX_SPEED);
    _stepper.setAcceleration(STEPPER_ACCELERATION);

    Serial.printf("[Stepper step:%d dir:%d] Ready\n",
                  _stepper.currentPosition(), _stepper.currentPosition());
}

// ── NEW: move by a target angle in degrees ────────────────────────────────────
// Positive angle = CW, negative = CCW (mirrors your snippet's convention).
// Converts to absolute steps so AccelStepper handles acceleration/direction.
void StepperMotor::moveToAngle(float degrees) {
    long steps = degreesToSteps(degrees);
    _stepper.moveTo(steps);
    Serial.printf("[Stepper] Angle %.2f° → %ld steps (gear %.0f:1)\n",
                  degrees, steps, GEAR_RATIO);
}

// ── NEW: returns current shaft angle in degrees ───────────────────────────────
float StepperMotor::getAngle()  {
    return stepsToDegrees(_stepper.currentPosition());
}

void StepperMotor::moveTo(long position) {
    _stepper.moveTo(position);
    Serial.printf("[Stepper] Target: %ld steps\n", position);
}

void StepperMotor::run() {
    _stepper.run();
}

void StepperMotor::stop() {
    _stepper.stop();
}

long StepperMotor::getPosition() {
    return _stepper.currentPosition();
}

bool StepperMotor::isRunning() {
    return _stepper.isRunning();
}

// ── Private helpers ───────────────────────────────────────────────────────────
long StepperMotor::degreesToSteps(float degrees) const {
    // steps = angle / 360 * stepsPerRev * gearRatio
    return static_cast<long>(degrees / 360.0f * STEPS_PER_REV * GEAR_RATIO);
}

float StepperMotor::stepsToDegrees(long steps) const {
    return static_cast<float>(steps) * 360.0f / (STEPS_PER_REV * GEAR_RATIO);
}