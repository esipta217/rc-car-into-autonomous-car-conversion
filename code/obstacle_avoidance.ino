#include <Servo.h>

// Motor pins
#define ENA 10
#define IN1 3
#define IN2 4

// Steering motor pins
#define ENB 11
#define IN3 5
#define IN4 6

// Ultrasonic pins
#define TRIG 9
#define ECHO 12

// Servo pin
#define SERVO_PIN 13

// IR sensors at back
#define IR_LEFT 7
#define IR_RIGHT 8

Servo scanServo;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);


  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  scanServo.attach(SERVO_PIN);
  scanServo.write(90); // start center

  Serial.begin(115200);

  // Wait 5 seconds before starting
  Serial.println("System initializing... Starting in 5 seconds.");
  delay(5000);
}

// Returns distance in cm from ultrasonic sensor
int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) return 300; // no reading
  int distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  int frontDistance = getDistance();
  int irLeft = digitalRead(IR_LEFT);
  int irRight = digitalRead(IR_RIGHT);

  Serial.print("Front: "); Serial.print(frontDistance);
  Serial.print(" cm, IR Left: "); Serial.print(irLeft);
  Serial.print(", IR Right: "); Serial.println(irRight);

  // Both front and back have obstacles
  if (frontDistance <= 20 && irLeft == HIGH && irRight == HIGH) {
    stopMotor();
    Serial.println("Obstacle in front AND back! Deciding turn...");
    turnInClearDirection(); // Custom function
  } 
  // Only front blocked
  else if (frontDistance <= 20) {
    stopMotor();
    getUnstuck();
  } 
  // Only back blocked (more weightage to IR)
  else if (irLeft == HIGH && irRight == HIGH) {
    stopMotor();
    Serial.println("Back IR blocked! Avoid reversing. Proceed cautiously.");
    goForwardPulseWithScan();
  } 
  else {
    goForwardPulseWithScan();
  }
}

// Pulsed forward with servo scanning sweep during off phase
void goForwardPulseWithScan() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 150);
  delay(600);

  stopMotor();

  const int stepAngle = 10;
  const int steps = 180 / stepAngle;
  const int stepDelay = 600 / (steps * 2);

  for (int angle = 0; angle <= 180; angle += stepAngle) {
    scanServo.write(angle);
    delay(stepDelay);
  }
  for (int angle = 180; angle >= 0; angle -= stepAngle) {
    scanServo.write(angle);
    delay(stepDelay);
  }
}

void stopMotor() {
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void getUnstuck() {
  Serial.println("Attempting to get unstuck...");

  int distLeft, distCenter, distRight;

  scanServo.write(0);
  delay(400);
  distLeft = getDistance();
  Serial.print("Left distance: "); Serial.println(distLeft);

  scanServo.write(90);
  delay(400);
  distCenter = getDistance();
  Serial.print("Center distance: "); Serial.println(distCenter);

  scanServo.write(180);
  delay(400);
  distRight = getDistance();
  Serial.print("Right distance: "); Serial.println(distRight);

  // Back up until front clears or IRs detect obstacle
  while (true) {
    int irLeft = digitalRead(IR_LEFT);
    int irRight = digitalRead(IR_RIGHT);
    int frontDistance = getDistance();

    if (frontDistance > 20) {
      Serial.println("Front cleared during backup. Stop backing.");
      break;
    }

    if (irLeft == HIGH && irRight == HIGH) {
      Serial.println("Back blocked. Cannot reverse more.");
      stopMotor();
      break;
    }

    Serial.println("Backing up...");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 150);
    delay(600);
    stopMotor();
    delay(600);
  }

  // Decide turn direction
  if (distLeft > distRight) {
    Serial.println("Turning left to get unstuck...");
    turnLeftPulse();
  } else {
    Serial.println("Turning right to get unstuck...");
    turnRightPulse();
  }

  Serial.println("Trying forward after turn");
  goForwardPulseWithScan();
}

// If both front and back are blocked
void turnInClearDirection() {
  int distLeft, distRight;

  scanServo.write(0);
  delay(400);
  distLeft = getDistance();

  scanServo.write(180);
  delay(400);
  distRight = getDistance();

  if (distLeft > distRight) {
    Serial.println("Clearer on left — turning left");
    turnLeftPulse();
  } else {
    Serial.println("Clearer on right — turning right");
    turnRightPulse();
  }

  goForwardPulseWithScan();
}

void turnRightPulse() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 150);
  delay(500);
  stopMotor();

  scanServo.write(180);
  delay(400);
  scanServo.write(90);
  delay(200);
}

void turnLeftPulse() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 150);
  delay(500);
  stopMotor();

  scanServo.write(0);
  delay(400);
  scanServo.write(90);
  delay(200);
}
