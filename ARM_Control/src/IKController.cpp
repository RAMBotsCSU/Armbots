#include "IKController.h"
#include <Arduino.h>
#include <math.h>

IKController::IKController(ArmController& arm) : _arm(arm) {}

void IKController::applyDelta(float dr, float dz) {
    _r = constrain(_r + dr, R_MIN, R_MAX);
    _z = constrain(_z + dz, Z_MIN, Z_MAX);
    apply();
}

void IKController::setPhi(float phiDeg) {
    _phi = constrain(phiDeg, -90.0f, 90.0f);
    apply();
}

void IKController::reset() {
    _r   = 600.0f;
    _z   = 200.0f;
    _phi = 0.0f;
}

void IKController::apply() {
    Angles a = solveIK(_r, _z, _phi * DEG2RAD);
    _arm.setShoulder((int)lroundf(a.shoulder));
    _arm.setElbow   ((int)lroundf(a.elbow));
    _arm.setWrist   ((int)lroundf(a.wrist));
    Serial.printf("[IK] r=%.0f z=%.0f phi=%.0f -> sh=%.1f el=%.1f wr=%.1f\n",
                  _r, _z, _phi, a.shoulder, a.elbow, a.wrist);
}

IKController::Angles IKController::solveIK(float r, float z, float phiRad) {
    // Wrist centre — subtract last link along approach direction
    float wx = r - L3 * cosf(phiRad);
    float wz = z - L3 * sinf(phiRad);

    // Clamp wrist centre to reachable workspace
    float dist     = sqrtf(wx*wx + wz*wz);
    float maxReach = L1 + L2 - 1.0f;
    float minReach = fabsf(L1 - L2) + 1.0f;

    if      (dist > maxReach) { float s = maxReach / dist; wx *= s; wz *= s; }
    else if (dist < minReach) { float s = minReach / dist; wx *= s; wz *= s; }

    // 2-link IK for shoulder (q1) and elbow (q2)
    float D = (wx*wx + wz*wz - L1*L1 - L2*L2) / (2.0f * L1 * L2);
    D = fmaxf(-1.0f, fminf(1.0f, D));

    float sinQ2 = fmaxf(0.0f, 1.0f - D*D);
    float q2    = atan2f(sqrtf(sinQ2), D);
    float q1    = atan2f(wz, wx) - atan2f(L2 * sinf(q2), L1 + L2 * cosf(q2));

    if (q1 < 0.0f) {
        q2 = atan2f(-sqrtf(sinQ2), D);
        q1 = atan2f(wz, wx) - atan2f(L2 * sinf(q2), L1 + L2 * cosf(q2));
    }

    q1 = fmaxf(0.0f, fminf(PI_F, q1));
    float q3 = phiRad - q1 - q2;

    return {
        q1 * RAD2DEG,
        fabsf(q2 * RAD2DEG - OFFSET2_DEG),
        fabsf(q3 * RAD2DEG - OFFSET3_DEG)
    };
}
