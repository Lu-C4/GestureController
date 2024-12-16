#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "./MotorController.h"

// Define pins for the RF24 module
#define CE_PIN 6
#define CSN_PIN 10

MotorController motorController(9, 8, 7, 3, 5, 4);

RF24 radio(CE_PIN, CSN_PIN);  // Create RF24 object with custom CE and CSN pins
const byte address[6] = "00001";  // Same address as the sender


void setup() {
    Serial.begin(9600);

    motorController.begin(200); // Set speed to 200
    
    // Initialize the RF24 module
    if (!radio.begin()) {
        Serial.println("Radio initialization failed!");
        while (1);  // Halt if initialization fails
    }

    radio.openReadingPipe(0, address);  // Open a reading pipe on the same address as the sender
    radio.setPALevel(RF24_PA_HIGH); 
    radio.setDataRate(RF24_250KBPS);    // Set data rate to 250 Kbps
    radio.setChannel(108);              // Use the same channel as the sender
    radio.startListening();             // Start listening for incoming messages

    Serial.println("RF24 Receiver Initialized and Listening");
}

void loop() {
    if (radio.available()) {  // Check if data is available
        char payload[1] = "";  // Buffer to store received data
        radio.read(&payload, sizeof(payload));  // Read the data
        Serial.println("Received: " + String(payload));  // Print the received data
        motorController.updateState(payload[0]);
    }
}
