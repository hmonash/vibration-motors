#include <Arduino.h>

/*
  Ultrasonic Sensor to Vibration Motor Controller (Direct Transistor/PWM Edition)
  Connects an HC-SR04 ultrasonic sensor to a vibration motor via a transistor.
  Uses Pin 3 (PWM) for the vibration motor control.
  When an object is detected within 50cm, the vibration motor activates.
*/

const int trigPin = 9;    // HC-SR04 Trigger pin
const int echoPin = 10;   // HC-SR04 Echo pin
const int motorPin = 3;   // PWM pin connected to transistor base/gate

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
  Serial.println("Direct PWM Motor Controller Initializing...");
  Serial.flush();

  Serial.println("Initializing pins...");
  Serial.flush();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorPin, OUTPUT);

  // Ensure motor is off
  analogWrite(motorPin, 0);

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

    analogWrite(motorPin, motorSpeed);
  } else {
    // Turn off motor
    analogWrite(motorPin, 0);
  }

  delay(50); // Faster sampling for better responsiveness
}
