#ifndef BALLGRABROUTINE_H
#define BALLGRABROUTINE_H
// NEEDS TESTING 

#include "ArmController.h"
#include <Arduino.h>
#include <math.h>

class BallGrabRoutine {
public:
    BallGrabRoutine(ArmController& arm);
    void run();

private:
    ArmController& arm_;

    static constexpr float L1 = 402.5f;
    static constexpr float L2 = 399.862f;
    static constexpr float L3 = 145.225f;
    static constexpr float OFFSET2_DEG = 56.9434427f;
    static constexpr float OFFSET3_DEG = 44.82008871f;
    static constexpr float PHI = 0.0f;
    static constexpr int   PATH_POINTS = 120;
    static constexpr float PI_F = 3.14159265358979323846f;
    static constexpr float RAD2DEG = 180.0f / PI_F;

    struct JointAngles {
        float shoulder;
        float elbow;
        float wrist;
    };

    JointAngles computeIk(float x, float y, float z);
    void waitForArm(float seconds);
    void runPath(float path[][3], int count);
};

#endif // BALLGRABROUTINE_H