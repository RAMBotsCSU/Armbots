#include "StepperMotor.h"
#include "config.h"
#include <Arduino.h>

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
