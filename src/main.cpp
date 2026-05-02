#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

/*
  Ultrasonic Sensor to Vibration Motor Controller (Motor Shield V2 Edition)
  Connects an HC-SR04 ultrasonic sensor to a TB6612/PCA9685 Motor Shield.
  Uses DC Motor ports 1 and 2 for the vibration motors.
  When an object is detected within 30cm, the vibration motors activate.
*/

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 and M4.
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);

const int trigPin = 9;    // HC-SR04 Trigger pin
const int echoPin = 10;   // HC-SR04 Echo pin

// Configuration
const int maxDistance = 50; // Maximum distance to react (cm)
const int minDistance = 5;  // Minimum distance for max vibration (cm)

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  if (duration == 0) return 999; 
  
  long distance = duration * 0.034 / 2; // Distance in cm
  return distance;
}

void setup() {
  Serial.begin(9600);
  Serial.println("4-Motor Ultrasonic Controller Initializing...");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Initialize all motors to off
  motor1->run(RELEASE);
  motor2->run(RELEASE);
  motor3->run(RELEASE);
  motor4->run(RELEASE);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  long distance = getDistance();
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance < maxDistance) {
    // Map distance to speed (closer = faster)
    // Constrain distance to our range first
    int constrainedDist = constrain(distance, minDistance, maxDistance);
    int motorSpeed = map(constrainedDist, maxDistance, minDistance, 80, 255);

    motor1->setSpeed(motorSpeed);
    motor2->setSpeed(motorSpeed);
    motor3->setSpeed(motorSpeed);
    motor4->setSpeed(motorSpeed);

    motor1->run(FORWARD);
    motor2->run(FORWARD);
    motor3->run(FORWARD);
    motor4->run(FORWARD);
  } else {
    // Turn off all motors
    motor1->run(RELEASE);
    motor2->run(RELEASE);
    motor3->run(RELEASE);
    motor4->run(RELEASE);
  }

  delay(50); // Faster sampling for better responsiveness
}
