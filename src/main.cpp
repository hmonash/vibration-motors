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
  Serial.begin(115200);
  delay(2000); // Give the serial monitor time to connect
  
  Serial.println("\n\n--- Starting System ---");
  Serial.println("Motor Ultrasonic Controller Initializing...");
  Serial.flush();

  Serial.println("Attempting to initialize Motor Shield...");
  Serial.flush();
  
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    Serial.flush();
    while (1);
  }
  Serial.println("Motor Shield found.");
  Serial.flush();

  // Initialize motor to off
  Serial.println("Initializing motor...");
  Serial.flush();
  motor1->run(RELEASE);

  Serial.println("Initializing pins...");
  Serial.flush();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("Initialization complete!");
  Serial.flush();
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
    motor1->run(FORWARD);
  } else {
    // Turn off motor
    motor1->run(RELEASE);
  }

  delay(50); // Faster sampling for better responsiveness
}
