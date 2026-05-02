# Ultrasonic 4-Motor Vibration Controller

This project uses an Arduino Uno, an Adafruit Motor Shield V2 (TB6612/PCA9685), and an HC-SR04 ultrasonic sensor to control four mini vibration motors. The vibration intensity is mapped to the distance measured by the sensor.

## Features
- Controls 4 DC motors (vibration motors) via I2C.
- Proportional feedback: Vibration strength increases as objects get closer.
- Detection range: 5cm (max vibration) to 50cm (min vibration).

## Hardware Requirements
- Arduino Uno
- TB6612/PCA9685 Motor Shield (Standard I2C V2)
- HC-SR04 Ultrasonic Sensor
- 4x Mini Vibration Motors
- 9V Power Supply (connected to Arduino barrel jack)

## Wiring
- **Motors:** Connected to ports M1, M2, M3, and M4 on the shield.
- **Ultrasonic Sensor:**
  - VCC -> 5V
  - Trig -> Pin 9
  - Echo -> Pin 10
  - GND -> GND
- **Power:** Ensure the "VIN Jumper" is placed on the motor shield to share power from the Arduino.

## Software Dependencies
- [Adafruit Motor Shield V2 Library](https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library)
- [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)

## Usage
1. Open the project in VS Code with PlatformIO.
2. Connect your Arduino Uno via USB.
3. Build and Upload the code.
4. Use the Serial Monitor (9600 baud) to view distance readings.
