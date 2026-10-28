/*
 * ============================================================================
 * Project: 3-Axis Robotic Arm - Calibration & Sweep Test
 * Purpose: Matches the behavior seen in Video 1 (automated sweep back & forth)
 *          to verify mechanical range of motion and servo health.
 * ============================================================================
 */

#include <Servo.h>

const int PIN_SERVO_BASE = 9;   // Servo 1: Base / Shoulder
const int PIN_SERVO_ARM  = 10;  // Servo 2: Arm / Elbow

Servo servoBase;
Servo servoArm;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Starting Servo Sweep Test..."));

  servoBase.attach(PIN_SERVO_BASE);
  servoArm.attach(PIN_SERVO_ARM);

  // Home position
  servoBase.write(90);
  servoArm.write(90);
  delay(1000);
}

void loop() {
  // Sweep from 10 degrees to 170 degrees
  Serial.println(F("Sweeping Forward: 10 -> 170 deg"));
  for (int pos = 10; pos <= 170; pos += 1) {
    servoBase.write(pos);
    delay(15); // Adjust speed of sweep
  }
  delay(500);

  // Sweep back from 170 degrees to 10 degrees
  Serial.println(F("Sweeping Backward: 170 -> 10 deg"));
  for (int pos = 170; pos >= 10; pos -= 1) {
    servoBase.write(pos);
    delay(15);
  }
  delay(500);
}
