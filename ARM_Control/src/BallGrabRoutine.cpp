#include "BallGrabRoutine.h"
#include <Arduino.h>
#include <math.h>

BallGrabRoutine::BallGrabRoutine(ArmController& arm) : arm_(arm) {}

BallGrabRoutine::JointAngles BallGrabRoutine::computeIk(float x, float y, float z) {
    float px = sqrtf(x * x + y * y);
    float pz = z;

    float wx = px - L3 * cosf(PHI);
    float wz = pz - L3 * sinf(PHI);

    float dist = sqrtf(wx * wx + wz * wz);
    float maxReach2 = L1 + L2 - 1.0f;
    float minReach2 = fabsf(L1 - L2) + 1.0f;

    if (dist > maxReach2) {
        float scale = maxReach2 / dist;
        wx *= scale; wz *= scale; dist = maxReach2;
    } else if (dist < minReach2) {
        float scale = minReach2 / dist;
        wx *= scale; wz *= scale; dist = minReach2;
    }

    float D = (wx * wx + wz * wz - L1 * L1 - L2 * L2) / (2.0f * L1 * L2);
    D = fmaxf(-1.0f, fminf(1.0f, D));

    float oneMinusD2 = fmaxf(0.0f, 1.0f - D * D);
    float q2 = atan2f(sqrtf(oneMinusD2), D);
    float q1 = atan2f(wz, wx) - atan2f(L2 * sinf(q2), L1 + L2 * cosf(q2));

    if (q1 < 0.0f) {
        q2 = atan2f(-sqrtf(oneMinusD2), D);
        q1 = atan2f(wz, wx) - atan2f(L2 * sinf(q2), L1 + L2 * cosf(q2));
    }

    q1 = fmaxf(0.0f, fminf(PI_F, q1));
    float q3 = PHI - q1 - q2;

    float shoulder_deg = q1 * RAD2DEG;
    float q2_eff_deg   = (q2 * RAD2DEG) - OFFSET2_DEG;
    float q3_eff_deg   = (q3 * RAD2DEG) - OFFSET3_DEG;

    return { shoulder_deg, fabsf(q2_eff_deg), fabsf(q3_eff_deg) };
}

void BallGrabRoutine::waitForArm(float seconds) {
    unsigned long deadline = millis() + (unsigned long)(seconds * 1000.0f);
    while (millis() < deadline) {
        arm_.run();
        delay(10);
    }
}

void BallGrabRoutine::runPath(float path[][3], int count) {
    for (int i = 0; i < count; ++i) {
        JointAngles angles = computeIk(path[i][0], path[i][1], path[i][2]);
        arm_.setShoulder((int)lroundf(angles.shoulder));
        arm_.setElbow((int)lroundf(angles.elbow));
        arm_.setWrist((int)lroundf(angles.wrist));

        // allow smooth motion
        for (int step = 0; step < 5; ++step) {
            arm_.run();
            delay(50);
        }
    }
}

void BallGrabRoutine::run() {
    Serial.println("[BallGrabRoutine] Starting...");

    arm_.home();
    waitForArm(2.0f);

    float path[PATH_POINTS][3];
    const float start[3] = {800.0f, 0.0f, 200.0f};
    const float end[3]   = {800.0f, 0.0f, -100.0f};

    for (int i = 0; i < PATH_POINTS; ++i) {
        float t = (float)i / (float)(PATH_POINTS - 1);
        path[i][0] = (1.0f - t) * start[0] + t * end[0];
        path[i][1] = (1.0f - t) * start[1] + t * end[1];
        path[i][2] = (1.0f - t) * start[2] + t * end[2];
    }

    // Move to start point
    JointAngles first = computeIk(start[0], start[1], start[2]);
    Serial.printf("[BallGrabRoutine] Move to start: sh=%.4f el=%.4f wr=%.4f\n",
                  first.shoulder, first.elbow, first.wrist);
    arm_.setShoulder((int)lroundf(first.shoulder));
    arm_.setElbow((int)lroundf(first.elbow));
    arm_.setWrist((int)lroundf(first.wrist));
    waitForArm(4.0f);

    // Draw downward
    Serial.println("[BallGrabRoutine] Drawing downward...");
    runPath(path, PATH_POINTS);

    // Tighten gripper at bottom
    Serial.println("[BallGrabRoutine] Tightening gripper...");
    arm_.moveGripperToAngle(30); // existing API
    waitForArm(2.0f);

    // Reverse back up
    Serial.println("[BallGrabRoutine] Reversing upward...");
    for (int i = PATH_POINTS - 1; i >= 0; --i) {
        JointAngles angles = computeIk(path[i][0], path[i][1], path[i][2]);
        arm_.setShoulder((int)lroundf(angles.shoulder));
        arm_.setElbow((int)lroundf(angles.elbow));
        arm_.setWrist((int)lroundf(angles.wrist));
        for (int step = 0; step < 5; ++step) {
            arm_.run();
            delay(50);
        }
    }

    Serial.println("[BallGrabRoutine] Done.");
}