# Air Mouse (ESP32 + MPU6050)

A compact wireless air mouse that lets you control a computer cursor through hand movements in the air, with no desk or flat surface required.

## How it works
An MPU6050 gyroscope measures hand rotation. The ESP32 calibrates the sensor, filters out noise and drift, and sends the motion to the computer as a Bluetooth Low Energy (BLE) mouse. Physical buttons provide left and right click.

- Gyro Z (yaw) controls horizontal cursor movement
- Gyro X (pitch) controls vertical cursor movement
- Axis mapping assumes the MPU6050 is mounted upside down (adjustable in `config.h`)

## Hardware
- ESP32 DevKit V1 (prototype), planned move to ESP32-C3 Super Mini
- MPU6050 accelerometer/gyroscope module
- 2 push buttons (left / right click)
- Rechargeable battery and charging circuit (planned)

## Wiring
| MPU6050 | ESP32 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| AD0 | GND |

Buttons: left click on GPIO 25, right click on GPIO 26, each wired to GND.

## Software
Built with PlatformIO (Arduino framework). Tunable settings (pins, sensitivity, deadzone, axis signs) are in `include/config.h`.

## Getting started
1. Open the project in VS Code with the PlatformIO extension
2. Build and upload to the ESP32
3. Keep the device still for about 3 seconds at boot (gyro calibration)
4. Pair "AirMouse" from your computer's Bluetooth settings

## Status
Work in progress: gyro calibration and axis mapping are done, and BLE mouse control is being tested.

## Roadmap
- Adjustable sensitivity and motion calibration
- Scrolling and extra buttons
- Battery level indication and power saving
- Gesture recognition
- Custom enclosure

## License
MIT