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
    delay(100);
    _wrist.begin();
    delay(100);
    _shoulder.begin();
    _shoulder.setSpeed(70.0f);          // max speed — 70°/sec
    _shoulder.setAcceleration(20.0f);   // ramp rate — lower = softer start/stop 
    // Possibly add some speed that could be not linear, goes slow to start and speeds up in the middle and slows to the end angle. 
    delay(100);

    Serial.println("[Arm] Initialising steppers...");
    _base.begin();
    _gripper.begin();

    Serial.println("[Arm] Ready.");
}

void ArmController::run() {
    // Servos interpolate toward their target angle each loop()
    _elbow.update();
    _shoulder.update();
    _wrist.update();

    // Steppers move one step at a time — must be called every loop()
    _base.run();
    _gripper.run();
}

void ArmController::home() {
    Serial.println("[Arm] Homing all joints (slow)...");
    _elbow.setSpeed(HOME_SERVO_SPEED);
    _shoulder.setSpeed(HOME_SERVO_SPEED);
    _wrist.setSpeed(HOME_SERVO_SPEED);
    _elbow.setAngle(15);
    _shoulder.setAngle(0);
    _wrist.setAngle(22);
    _base.moveTo(0);
    _gripper.moveTo(0);
}

bool ArmController::isAtHome() {
    return abs(_elbow.getAngle()    - 15) <= 1 &&
           abs(_shoulder.getAngle() -  0) <= 1 &&
           abs(_wrist.getAngle()    - 22) <= 1 &&
           !_base.isRunning()              &&
           !_gripper.isRunning();
}

void ArmController::restoreNormalSpeeds() {
    _elbow.setSpeed(90.0f);
    _shoulder.setSpeed(70.0f);
    _shoulder.setAcceleration(20.0f);
    _wrist.setSpeed(90.0f);
}

// ── Servo joints ──────────────────────────────────────────────────────────────
void ArmController::setElbow   (int angle) { _elbow.setAngle(angle);    }
void ArmController::setShoulder(int angle) { _shoulder.setAngle(angle); }
void ArmController::setWrist   (int angle) { _wrist.setAngle(angle);    }

void ArmController::setElbowSpeed   (float d) { _elbow.setSpeed(d);    }
void ArmController::setShoulderSpeed(float d) { _shoulder.setSpeed(d); }
void ArmController::setWristSpeed   (float d) { _wrist.setSpeed(d);    }

// ── Stepper joints ────────────────────────────────────────────────────────────
void ArmController::moveBase   (long pos) { _base.moveTo(pos);    }
void ArmController::moveGripper(long pos) { _gripper.moveTo(pos); }
void ArmController::stopBase()            { _base.stop();         }
void ArmController::stopGripper()         { _gripper.stop();      }

// ── Stepper angle control (NEW) ─────────────────────────────────────────────────────  
void  ArmController::moveBaseToAngle(float deg)    { _base.moveToAngle(deg);     }
void  ArmController::moveGripperToAngle(float deg) { _gripper.moveToAngle(deg);  }

// ── State getters ─────────────────────────────────────────────────────────────
int  ArmController::getElbow()      const { return _elbow.getAngle();       }
int  ArmController::getShoulder()   const { return _shoulder.getAngle();    }
int  ArmController::getWrist()      const { return _wrist.getAngle();       }
long ArmController::getBasePos()    { return _base.getPosition();     }
long ArmController::getGripperPos() { return _gripper.getPosition();  }
float ArmController::getBaseAngle()    { return _base.getAngle();     }  // These are new
float ArmController::getGripperAngle() { return _gripper.getAngle();  }  