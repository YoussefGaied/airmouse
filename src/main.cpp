#include <Arduino.h>
#include <Wire.h>
#include <BleMouse.h>
#include "config.h"

const uint8_t MPU_ADDR = 0x68;

BleMouse bleMouse("AirMouse", "DIY", 100);

float biasX = 0, biasZ = 0;
float remX = 0, remY = 0;          // fractional leftovers between loops
unsigned long lastMs = 0;
bool leftDown = false, rightDown = false;

bool readGyroRaw(int16_t &gx, int16_t &gy, int16_t &gz) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(MPU_ADDR, (uint8_t)6) != 6) return false;
  gx = (Wire.read() << 8) | Wire.read();
  gy = (Wire.read() << 8) | Wire.read();
  gz = (Wire.read() << 8) | Wire.read();
  return true;
}

void calibrateGyro(int samples = 1000) {
  Serial.println("Calibrating... keep the board still");
  long sx = 0, sz = 0;
  int good = 0;
  while (good < samples) {
    int16_t gx, gy, gz;
    if (!readGyroRaw(gx, gy, gz)) { delay(3); continue; }
    sx += gx; sz += gz;
    good++;
    delay(3);
  }
  biasX = sx / (float)samples;
  biasZ = sz / (float)samples;
  Serial.println("Calibration done.");
}

float applyDeadzone(float v) {
  return (fabsf(v) < DEADZONE) ? 0.0f : v;
}

int8_t clamp8(int v) {
  if (v > 127) return 127;
  if (v < -127) return -127;
  return (int8_t)v;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
  pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);

  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(100000);

  Wire.beginTransmission(MPU_ADDR);      // wake up
  Wire.write(0x6B); Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(MPU_ADDR);      // ±500 deg/s
  Wire.write(0x1B); Wire.write(0x08);
  Wire.endTransmission();

  delay(2000);
  calibrateGyro();

  bleMouse.begin();
  Serial.println("BLE mouse started. Pair 'AirMouse' from your computer.");
  lastMs = millis();
}

void loop() {
  unsigned long now = millis();
  float dt = (now - lastMs) / 1000.0f;
  lastMs = now;

  int16_t gx, gy, gz;
  if (!readGyroRaw(gx, gy, gz)) { delay(5); return; }

  // Mapping (MPU upside down): cursor X <- gyro Z, cursor Y <- gyro X
  float rateH = applyDeadzone((gz - biasZ) / GYRO_SCALE);
  float rateV = applyDeadzone((gx - biasX) / GYRO_SCALE);

  if (bleMouse.isConnected()) {
    float fx = SIGN_X * rateH * SENSITIVITY * dt + remX;
    float fy = SIGN_Y * rateV * SENSITIVITY * dt + remY;
    int dx = (int)fx;
    int dy = (int)fy;
    remX = fx - dx;
    remY = fy - dy;
    if (dx != 0 || dy != 0) bleMouse.move(clamp8(dx), clamp8(dy));

    bool l = (digitalRead(PIN_BTN_LEFT) == LOW);
    bool r = (digitalRead(PIN_BTN_RIGHT) == LOW);
    if (l && !leftDown)  bleMouse.press(MOUSE_LEFT);
    if (!l && leftDown)  bleMouse.release(MOUSE_LEFT);
    if (r && !rightDown) bleMouse.press(MOUSE_RIGHT);
    if (!r && rightDown) bleMouse.release(MOUSE_RIGHT);
    leftDown = l;
    rightDown = r;
  }
  delay(5);
}