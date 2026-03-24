#pragma once
#include "StandardServo.h"
#include "DockyeServo.h"
#include "StepperMotor.h"

// ─── ArmController ────────────────────────────────────────────────────────────
// Single interface for the entire robot arm.
// All motor objects live here — nothing else in the project creates motors.
//
// Servo joints  → angle in degrees (0–180)
// Stepper joints → absolute step position
//
// Usage:
//   ArmController arm;
//   arm.begin();
//   arm.setElbow(90);
//   arm.moveBase(500);
//   arm.run();  // must be called every loop()

class ArmController {
public:
    ArmController();

    void begin();  // initialise all motors
    void run();    // call every loop() — keeps steppers moving
    void home();   // return all joints to zero position

    // ── Servo joints ────────────────────────────────────────────────────────
    void setElbow   (int angle);   // 0–180°
    void setShoulder(int angle);   // 0–180°  (Dockye servo)
    void setWrist   (int angle);   // 0–180°

    // ── Stepper joints ───────────────────────────────────────────────────────
    void moveBase   (long position); // absolute step position
    void moveGripper(long position); // absolute step position
    void stopBase();
    void stopGripper();

    // ── State getters ────────────────────────────────────────────────────────
    int  getElbow()      const;
    int  getShoulder()   const;
    int  getWrist()      const;
    long getBasePos()    const;
    long getGripperPos() const;

private:
    StandardServo _elbow;
    DockyeServo   _shoulder;
    StandardServo _wrist;
    StepperMotor  _base;
    StepperMotor  _gripper;
};
