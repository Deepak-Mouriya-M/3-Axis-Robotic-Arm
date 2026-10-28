# 3-Axis Robotic Arm

An Arduino Uno-powered 3-axis robotic arm driven by SG90 micro servos and controlled via a 2-axis analog thumb joystick. Features real-time multi-axis movement with smooth rate control, position holding, and pushbutton gripper toggling for pick-and-place tasks.

---

## Hardware Components
- **Arduino Uno** (Microcontroller)
- **TowerPro SG90 9g Micro Servos** (Joint actuators)
- **Analog Thumb Joystick Module** (KY-023 Dual-Axis with Pushbutton)
- **Breadboard & Jumper Wires**
- **5V Power Supply**

---

## Circuit Connections

| Component | Pin / Wire | Arduino Uno Pin | Description |
| :--- | :--- | :--- | :--- |
| **Joystick** | GND | GND | Ground |
| | +5V | 5V | Power |
| | VRx | A0 | Base / Axis 1 Control |
| | VRy | A1 | Arm / Axis 2 Control |
| | SW | Pin 2 | Claw / Gripper Pushbutton |
| **Servo 1 (Base)** | Signal (Orange) | Pin 9 | Base Joint PWM |
| | VCC (Red) / GND (Brown) | 5V / GND | Power |
| **Servo 2 (Arm)** | Signal (Orange) | Pin 10 | Elbow / Arm PWM |
| | VCC (Red) / GND (Brown) | 5V / GND | Power |
| **Servo 3 (Gripper)**| Signal (Orange) | Pin 11 | Claw / Gripper PWM |
| | VCC (Red) / GND (Brown) | 5V / GND | Power |

---

## Code Files
- `Robotic_Arm_Joystick_Control.ino`: Main project sketch featuring smooth rate control, deadzone filtering, and position-holding functionality.
- `Servo_Sweep_Test.ino`: Range and calibration test code to verify mechanical motion and servo health.

---

## Media
- Demonstrations and build setup videos & images are included in this repository.
