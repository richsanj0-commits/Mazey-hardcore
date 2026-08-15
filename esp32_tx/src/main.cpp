#include <Arduino.h>

// ESP32 Pins connected to HC-12:
// RX2 (GPIO16) -> HC-12 TXD
// TX2 (GPIO17) -> HC-12 RXD
#define RXD2 16
#define TXD2 17

unsigned long lastSendTime = 0;
int packetCount = 0;

void setup() {
  // Serial Monitor for debug
  Serial.begin(115200);
  while (!Serial && millis() < 1000);
  Serial.println("ESP32 Transmitter Initialized.");

  // HardwareSerial 2 connected to HC-12
  // Default baud rate for HC-12 is 9600
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("HC-12 Serial2 (9600 baud) started.");
}

void loop() {
  // Send data every 1 second
  if (millis() - lastSendTime >= 1000) {
    lastSendTime = millis();
    packetCount++;

    // Prepare packet message
    String msg = "PING Count: " + String(packetCount) + "\n";
    
    // Send to HC-12 (over Serial2)
    Serial2.print(msg);

    // Print to PC Serial monitor for tracking
    Serial.print("Sent: ");
    Serial.print(msg);
  }

  // Also read any data coming back from HC-12 (if Nano replies)
  while (Serial2.available() > 0) {
    char inChar = (char)Serial2.read();
    Serial.print(inChar);
  }
}
