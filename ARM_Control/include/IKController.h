#pragma once
#include "ArmController.h"

// ─── IKController ─────────────────────────────────────────────────────────────
// Real-time end-effector control via inverse kinematics.
//
// Maintains the current EE position as (r, z) in the arm's vertical plane:
//   r = radial reach from base axis (mm)
//   z = height above base (mm)
//
// Base rotation is handled separately by ArmController::moveBaseToAngle().
// IK solves for shoulder / elbow / wrist only.
//
// Link lengths from Python CAD model (ARM_IK_Solver.py):
//   L1 = 402.5 mm   L2 = 399.836 mm   L3 = 225.075 mm
//
// Usage:
//   ik.applyDelta(dr, dz);   // from joystick — updates position and commands arm
//   ik.setPhi(degrees);      // wrist approach angle — re-solves immediately
//   ik.reset();              // snap internal state back to safe default

class IKController {
public:
    IKController(ArmController& arm);

    void applyDelta(float dr, float dz);
    void setPhi(float phiDeg);
    void reset();

    float getR() const { return _r; }
    float getZ() const { return _z; }

private:
    ArmController& _arm;

    float _r   = 600.0f;
    float _z   = 200.0f;
    float _phi = 0.0f;

    static constexpr float L1          = 402.5f;
    static constexpr float L2          = 399.836f;
    static constexpr float L3          = 225.075f;
    static constexpr float OFFSET2_DEG = 56.9434427f;
    static constexpr float OFFSET3_DEG = 44.82008871f;
    static constexpr float PI_F        = 3.14159265358979f;
    static constexpr float RAD2DEG     = 180.0f / PI_F;
    static constexpr float DEG2RAD     = PI_F / 180.0f;

    static constexpr float R_MIN = 150.0f;
    static constexpr float R_MAX = 900.0f;
    static constexpr float Z_MIN = -300.0f;
    static constexpr float Z_MAX =  600.0f;

    struct Angles { float shoulder, elbow, wrist; };
    Angles solveIK(float r, float z, float phiRad);
    void   apply();
};
