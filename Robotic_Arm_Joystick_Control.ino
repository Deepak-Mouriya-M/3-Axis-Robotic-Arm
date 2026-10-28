/*
 * ============================================================================
 * Project: 3-Axis Robotic Arm - Joystick Control
 * Board:   Arduino Uno (or compatible)
 * Servos:  TowerPro SG90 9g Micro Servos
 * Control: 2-Axis Analog Thumb Joystick Module (KY-023)
 * ============================================================================
 * 
 * Circuit Connections:
 * --------------------
 * Joystick Module:
 *   - GND  -> Arduino GND
 *   - +5V  -> Arduino 5V
 *   - VRx  -> Arduino Analog Pin A0  (Controls Base / Axis 1)
 *   - VRy  -> Arduino Analog Pin A1  (Controls Arm Shoulder / Axis 2)
 *   - SW   -> Arduino Digital Pin 2  (Pushbutton to toggle Gripper open/close)
 * 
 * Servos (TowerPro SG90):
 *   - Servo 1 (Base / Shoulder): Signal (Orange) -> Arduino Pin 9
 *   - Servo 2 (Elbow / Arm):     Signal (Orange) -> Arduino Pin 10
 *   - Servo 3 (Claw / Gripper):  Signal (Orange) -> Arduino Pin 11 (Optional / 3rd Axis)
 *   - All Servos VCC (Red)       -> 5V (Breadboard power rail)
 *   - All Servos GND (Brown)     -> GND (Breadboard ground rail)
 * 
 * Note on Power:
 * When running multiple servos simultaneously, it is recommended to power
 * the servos using an external 5V (1A-2A) power supply, with the external
 * GND connected to the Arduino GND.
 * ============================================================================
 */

#include <Servo.h>

// --- Pin Definitions ---
const int PIN_JOY_X   = A0;  // Joystick X-axis analog input
const int PIN_JOY_Y   = A1;  // Joystick Y-axis analog input
const int PIN_JOY_SW  = 2;   // Joystick pushbutton switch

const int PIN_SERVO_BASE    = 9;   // Servo 1: Base / Main link
const int PIN_SERVO_ARM     = 10;  // Servo 2: Arm / Elbow joint
const int PIN_SERVO_GRIPPER = 11;  // Servo 3: Gripper / Claw (optional 3rd axis)

// --- Servo Objects ---
Servo servoBase;
Servo servoArm;
Servo servoGripper;

// --- Servo Angle Variables (Starting at center position) ---
float angleBase    = 90.0;
float angleArm     = 90.0;
int   angleGripper = 90.0;

// --- Angle Limits (Prevent mechanical jamming) ---
const int MIN_BASE_ANGLE = 0;
const int MAX_BASE_ANGLE = 180;

const int MIN_ARM_ANGLE  = 15;
const int MAX_ARM_ANGLE  = 165;

const int GRIPPER_OPEN   = 50;   // Open claw position
const int GRIPPER_CLOSE  = 130;  // Closed claw position
bool isGripperClosed     = false;

// --- Joystick Calibration & Deadzone ---
// Center rest value is typically around 512 (0 - 1023)
const int JOY_CENTER_MIN = 470;
const int JOY_CENTER_MAX = 550;
const float STEP_SPEED   = 1.5;   // Speed of movement per loop iteration

// Debounce for switch
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;

void setup() {
  Serial.begin(9600);
  Serial.println(F("=== 3-Axis Robotic Arm Initialized ==="));

  // Initialize joystick switch with internal pull-up resistor
  pinMode(PIN_JOY_SW, INPUT_PULLUP);

  // Attach servos to PWM pins
  servoBase.attach(PIN_SERVO_BASE);
  servoArm.attach(PIN_SERVO_ARM);
  servoGripper.attach(PIN_SERVO_GRIPPER);

  // Set servos to initial positions
  servoBase.write((int)angleBase);
  servoArm.write((int)angleArm);
  servoGripper.write(angleGripper);
  delay(500);
}

void loop() {
  // 1. Read Analog Joystick Values (0 - 1023)
  int xVal = analogRead(PIN_JOY_X);
  int yVal = analogRead(PIN_JOY_Y);

  // -------------------------------------------------------------
  // CONTROL AXIS 1: BASE / MAIN SHOULDER (X-Axis)
  // Position-Hold Rate Control:
  // Moving the stick away from center increases or decreases angle.
  // Releasing the stick to center HOLDS the current position.
  // -------------------------------------------------------------
  if (xVal > JOY_CENTER_MAX) {
    // Map speed proportionally to stick deflection
    float speedFactor = map(xVal, JOY_CENTER_MAX, 1023, 1, 3);
    angleBase += STEP_SPEED * speedFactor;
    if (angleBase > MAX_BASE_ANGLE) angleBase = MAX_BASE_ANGLE;
  } 
  else if (xVal < JOY_CENTER_MIN) {
    float speedFactor = map(xVal, JOY_CENTER_MIN, 0, 1, 3);
    angleBase -= STEP_SPEED * speedFactor;
    if (angleBase < MIN_BASE_ANGLE) angleBase = MIN_BASE_ANGLE;
  }

  // -------------------------------------------------------------
  // CONTROL AXIS 2: ARM / ELBOW (Y-Axis)
  // -------------------------------------------------------------
  if (yVal > JOY_CENTER_MAX) {
    float speedFactor = map(yVal, JOY_CENTER_MAX, 1023, 1, 3);
    angleArm += STEP_SPEED * speedFactor;
    if (angleArm > MAX_ARM_ANGLE) angleArm = MAX_ARM_ANGLE;
  } 
  else if (yVal < JOY_CENTER_MIN) {
    float speedFactor = map(yVal, JOY_CENTER_MIN, 0, 1, 3);
    angleArm -= STEP_SPEED * speedFactor;
    if (angleArm < MIN_ARM_ANGLE) angleArm = MIN_ARM_ANGLE;
  }

  // -------------------------------------------------------------
  // CONTROL AXIS 3: GRIPPER / CLAW (Pushbutton Toggle)
  // Pressing joystick button toggles claw open / close
  // -------------------------------------------------------------
  int reading = digitalRead(PIN_JOY_SW);
  if (reading == LOW && lastButtonState == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();
    isGripperClosed = !isGripperClosed;
    angleGripper = isGripperClosed ? GRIPPER_CLOSE : GRIPPER_OPEN;
    servoGripper.write(angleGripper);
    Serial.print(F("Gripper State: "));
    Serial.println(isGripperClosed ? F("CLOSED") : F("OPEN"));
  }
  lastButtonState = reading;

  // -------------------------------------------------------------
  // UPDATE SERVOS
  // -------------------------------------------------------------
  servoBase.write((int)angleBase);
  servoArm.write((int)angleArm);

  // Smooth delay to control refresh rate
  delay(20);
}
