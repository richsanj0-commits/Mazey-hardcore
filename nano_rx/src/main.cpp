#include <Arduino.h>
#include <SoftwareSerial.h>

// On Arduino Nano, we use SoftwareSerial for HC-12 to keep Hardware Serial free for debugging via USB
// HC-12 RXD -> Nano Pin 3 (through voltage divider) -> SoftwareSerial TX Pin
// HC-12 TXD -> Nano Pin 2 -> SoftwareSerial RX Pin
#define HC12_RX_PIN 2
#define HC12_TX_PIN 3

SoftwareSerial HC12(HC12_RX_PIN, HC12_TX_PIN);

void setup() {
  // Serial Monitor for PC debugging (Hardware Serial)
  Serial.begin(115200);
  while (!Serial && millis() < 1000);
  Serial.println("Arduino Nano Receiver Initialized.");

  // HC-12 default baud rate is 9600
  HC12.begin(9600);
  Serial.println("SoftwareSerial (9600 baud) for HC-12 started.");
}

void loop() {
  // Check if data is received from HC-12
  if (HC12.available() > 0) {
    String incomingStr = HC12.readStringUntil('\n');
    Serial.print("Received wirelessly: ");
    Serial.println(incomingStr);
    
    // Optional: Send a confirmation back to ESP32
    HC12.println("ACK: Received packet successfully!");
  }
}
