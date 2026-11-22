#pragma once

// I2C pins (classic ESP32 DevKit; will change on the C3)
#define PIN_SDA 21
#define PIN_SCL 22

// Gyro scale at ±500 deg/s
#define GYRO_SCALE 65.5f

// Axis mapping (MPU6050 mounted upside down):
//   cursor X (left/right) <- gyro Z
//   cursor Y (up/down)    <- gyro X
// Flip a sign to reverse that direction
#define SIGN_X  1
#define SIGN_Y  1

// Ignore tiny rotations (deg/s) to stop jitter and drift
#define DEADZONE     1.0f

// Cursor speed multiplier (adjustable later)
#define PIN_BTN_LEFT   25
#define PIN_BTN_RIGHT  26

#define SENSITIVITY  8.0f