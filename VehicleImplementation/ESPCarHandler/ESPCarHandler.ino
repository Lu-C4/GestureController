#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "./MPU6050Handler.h"

// Define pins for the RF24 module
#define CE_PIN 27
#define CSN_PIN 26

RF24 radio(CE_PIN, CSN_PIN);      // CE and CSN pins
const byte address[6] = "00001";  // Communication address
MPU6050Handler mpuHandler;

// Persistent `prev` variable
char prev = 'S';

void setup() {
  Serial.begin(115200);

  // Initialize Radio
  if (!radio.begin()) {
    Serial.println("Radio initialization failed!");
    while (1)
      ;
  }

  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(108);
  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("RF24 Sender Initialized");

  // Initialize MPU6050 Handler
  if (!mpuHandler.initMPU()) {
    Serial.println("MPU6050 initialization failed!");
    while (1)
      ;
  }
  Serial.println("MPU6050 initialized successfully.");
}

void loop() {
  float yaw, pitch, roll;

  if (mpuHandler.readYPR(yaw, pitch, roll)) {
    char payload = 'S';  // Default state

    if (roll > 45) {
      payload = 'B';  // Backward
    } else if (roll < -45) {
      payload = 'F';  // Forward
    } else if (pitch > 45) {
      payload = 'L';  // Left
    } else if (pitch < -45) {
      payload = 'R';  // Right
    }

    // Only send if the payload has changed
    if (prev != payload) {
      Serial.println(payload);
      prev = payload;  // Update the previous state

      if (radio.write(&payload, sizeof(payload))) {
        Serial.println("Sent: " + String(payload));
      } else {
        Serial.println("Failed to send");
      }
    }
  }
  delay(10);  // Adjust delay for stability
}
