#include <Arduino.h>

// --- Pin Definitions based on Wiring Reference Sheet ---
// Buttons: PA2 = First Button, PA3 = Second Button
#define BTN_1 PA2 // First Button
#define BTN_2 PA3 // Second Button

// TB6612FNG Driver Standby Pin
#define STBY PB12

// Motor Speed PWM Pins
#define PWMA PA8  // Left Motor PWM
#define PWMB PA9  // Right Motor PWM

// Left Motor Direction Control Pins
#define AIN1 PB0
#define AIN2 PB1

// Right Motor Direction Control Pins
#define BIN1 PB2
#define BIN2 PB10

// Speed PWM value (0 to 255)
const int MOTOR_SPEED = 200;

void stopMotors() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void spinForward() {
  // Left Motor Forward: AIN1 = HIGH, AIN2 = LOW
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  // Right Motor Forward: BIN1 = LOW, BIN2 = HIGH
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  analogWrite(PWMA, MOTOR_SPEED);
  analogWrite(PWMB, MOTOR_SPEED);
}

void spinBackward() {
  // Left Motor Backward: AIN1 = LOW, AIN2 = HIGH
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  // Right Motor Backward: BIN1 = HIGH, BIN2 = LOW
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  analogWrite(PWMA, MOTOR_SPEED);
  analogWrite(PWMB, MOTOR_SPEED);
}

void setup() {
  // Configure button inputs with internal pull-up resistors
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);

  // Configure Motor Control Outputs
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Enable Motor Driver (STBY HIGH)
  digitalWrite(STBY, HIGH);

  // Initial state: stopped
  stopMotors();
}

void loop() {
  // Read button states (LOW when pressed due to INPUT_PULLUP)
  bool btn1Pressed = (digitalRead(BTN_1) == LOW); // PA2 (First button)
  bool btn2Pressed = (digitalRead(BTN_2) == LOW); // PA3 (Second button)

  if (btn1Pressed && !btn2Pressed) {
    spinForward();   // Both motors spin Forward on PA2
  } else if (btn2Pressed && !btn1Pressed) {
    spinBackward();  // Both motors spin Backward on PA3
  } else {
    stopMotors();
  }

  delay(10); // Small debounce/stability delay
}
