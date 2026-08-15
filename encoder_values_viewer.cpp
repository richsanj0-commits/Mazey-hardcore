#include <Arduino.h>

// ============================================================================
// Encoder Values Viewer for STM32 BlackPill (F401CC)
// ============================================================================
// Pin Definitions based on Wiring Reference Sheet:
// Left Motor Encoder: PA6 (Channel 1 / Phase A), PA7 (Channel 2 / Phase B)
// Right Motor Encoder: PA0 (Channel 1 / Phase A), PA1 (Channel 2 / Phase B)
// Reset Button: PA2 (BTN_1) - Resets encoder tick counts to 0
// Serial Output: USB CDC / Virtual COM Port (115200 baud)
// ============================================================================

#define BTN_1 PA2 
#define BTN_2 PA3 

#define STBY PB12

#define PWMA PA8  // Left Motor PWM
#define PWMB PA9  // Right Motor PWM

#define AIN1 PB0  // Left Motor Dir 1
#define AIN2 PB1  // Left Motor Dir 2

#define BIN1 PB2  // Right Motor Dir 1
#define BIN2 PB10 // Right Motor Dir 2

#define LEFT_ENC_A PA6
#define LEFT_ENC_B PA7

#define RIGHT_ENC_A PA0
#define RIGHT_ENC_B PA1

const int MOTOR_SPEED = 200;

// Quadrature Encoder Counters
volatile long leftEncoderTicks = 0;
volatile long rightEncoderTicks = 0;

// Interrupt Service Routine (ISR) for Left Motor Encoder
void handleLeftEncoder() {
  if (digitalRead(LEFT_ENC_B) == HIGH) {
    leftEncoderTicks++;
  } else {
    leftEncoderTicks--;
  }
}

// Interrupt Service Routine (ISR) for Right Motor Encoder
void handleRightEncoder() {
  if (digitalRead(RIGHT_ENC_B) == HIGH) {
    rightEncoderTicks++;
  } else {
    rightEncoderTicks--;
  }
}

void stopMotors() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void setup() {
  // Initialize USB Virtual COM Port (Serial Monitor)
  Serial.begin(115200);

  // Configure button inputs with internal pull-ups
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

  // Configure Encoder Pins with Internal Pull-ups
  pinMode(LEFT_ENC_A, INPUT_PULLUP);
  pinMode(LEFT_ENC_B, INPUT_PULLUP);
  pinMode(RIGHT_ENC_A, INPUT_PULLUP);
  pinMode(RIGHT_ENC_B, INPUT_PULLUP);

  // Attach Interrupts on Phase A rising edges
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_A), handleLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_A), handleRightEncoder, RISING);

  // Enable Motor Driver (STBY HIGH)
  digitalWrite(STBY, HIGH);

  // Keep motors stopped for manual wheel rotation testing
  stopMotors();
}

unsigned long lastPrintTime = 0;

void loop() {
  static bool lastBtn1State = false;

  // Read button states (LOW when pressed)
  bool btn1Pressed = (digitalRead(BTN_1) == LOW);

  // Button 1 (PA2): Reset encoder counts to 0 when pressed
  if (btn1Pressed && !lastBtn1State) {
    noInterrupts();
    leftEncoderTicks = 0;
    rightEncoderTicks = 0;
    interrupts();
    Serial.println("--- Encoders Reset to 0 ---");
  }
  lastBtn1State = btn1Pressed;

  // Motors remain stopped so you can manually turn the wheel for testing
  stopMotors();

  // Print Encoder Feedback to Serial Monitor every 100ms
  if (millis() - lastPrintTime >= 100) {
    lastPrintTime = millis();
    
    // Copy volatile counters atomically
    noInterrupts();
    long lPos = leftEncoderTicks;
    long rPos = rightEncoderTicks;
    interrupts();

    Serial.print("Left Encoder Feedback: ");
    Serial.print(lPos);
    Serial.print(" | Right Encoder Feedback: ");
    Serial.println(rPos);
  }
}
