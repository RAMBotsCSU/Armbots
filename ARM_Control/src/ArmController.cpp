#include "ArmController.h"
#include "config.h"
#include <Arduino.h>

ArmController::ArmController()
    : _elbow   (SERVO_ELBOW_PIN),
      _shoulder(SERVO_SHOULDER_PIN),
      _wrist   (SERVO_WRIST_PIN),
      _base    (BASE_ENABLE_PIN,    BASE_STEP_PIN,    BASE_DIR_PIN,
                BASE_MS1_PIN,       BASE_MS2_PIN,     BASE_MS3_PIN),
      _gripper (GRIPPER_ENABLE_PIN, GRIPPER_STEP_PIN, GRIPPER_DIR_PIN,
                GRIPPER_MS1_PIN,    GRIPPER_MS2_PIN,  GRIPPER_MS3_PIN)
{}

void ArmController::begin() {
    Serial.println("[Arm] Initialising servos...");
    _elbow.begin();
    _shoulder.begin();
    _wrist.begin();

    Serial.println("[Arm] Initialising steppers...");
    _base.begin();
    _gripper.begin();

    Serial.println("[Arm] Ready.");
}

void ArmController::run() {
    // Steppers move one step at a time — must be called every loop()
    _base.run();
    _gripper.run();
}

void ArmController::home() {
    Serial.println("[Arm] Homing all joints...");
    _elbow.setAngle(0);
    _shoulder.setAngle(0);
    _wrist.setAngle(0);
    _base.moveTo(0);
    _gripper.moveTo(0);
}

// ── Servo joints ──────────────────────────────────────────────────────────────
void ArmController::setElbow   (int angle) { _elbow.setAngle(angle);    }
void ArmController::setShoulder(int angle) { _shoulder.setAngle(angle); }
void ArmController::setWrist   (int angle) { _wrist.setAngle(angle);    }

// ── Stepper joints ────────────────────────────────────────────────────────────
void ArmController::moveBase   (long pos) { _base.moveTo(pos);    }
void ArmController::moveGripper(long pos) { _gripper.moveTo(pos); }
void ArmController::stopBase()            { _base.stop();         }
void ArmController::stopGripper()         { _gripper.stop();      }

// ── State getters ─────────────────────────────────────────────────────────────
int  ArmController::getElbow()      const { return _elbow.getAngle();       }
int  ArmController::getShoulder()   const { return _shoulder.getAngle();    }
int  ArmController::getWrist()      const { return _wrist.getAngle();       }
long ArmController::getBasePos()    const { return _base.getPosition();     }
long ArmController::getGripperPos() const { return _gripper.getPosition();  }
