#include <Arduino.h>
// PWM Pins
int step_one = 1;
int dir_1 = 2;
int enable_L = 4;
int dir_2 = 5;
int ms1_M2 = 9;
int ms2_M2 = 10;


// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}