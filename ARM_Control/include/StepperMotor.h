#pragma once
#include <AccelStepper.h>

// ─── StepperMotor ─────────────────────────────────────────────────────────────
// Wraps a single A4988-driven stepper motor.
// AccelStepper handles smooth acceleration/deceleration automatically.
//
// ⚠ IMPORTANT: call run() every loop() — it does one step at a time.
//   Missing calls = missed steps = wrong position.
//
// Usage:
//   StepperMotor myStepper(EN, STEP, DIR, MS1, MS2, MS3);
//   myStepper.begin();
//   myStepper.moveTo(500);   // non-blocking — keep calling run() in loop()
//   myStepper.run();

class StepperMotor {
public:
    StepperMotor(int enablePin, int stepPin, int dirPin,
                 int ms1Pin, int ms2Pin, int ms3Pin);

    void begin();               // configure pins and enable driver
    void moveTo(long position); // set target position (steps from home)
    void run();                 // MUST be called every loop() — executes steps
    void stop();                // halt immediately
    long getPosition() const;   // current position in steps
    bool isRunning() const;     // true if still moving toward target

private:
    AccelStepper _stepper;
    int _enablePin;
    int _ms1Pin, _ms2Pin, _ms3Pin;
};
