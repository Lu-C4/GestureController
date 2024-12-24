#include "./MPU6050Handler.h"

// Define input pins
#define INPUT_PIN1 4  // D4
#define INPUT_PIN2 5  // D5

MPU6050Handler mpuHandler;

unsigned long lastHeartbeatTime = 0; // Tracks the last time the heartbeat was sent
const unsigned long heartbeatInterval = 5000; // Interval for heartbeat (in milliseconds)

// Variables to store the previous button states
bool prevButtonStateLeft = HIGH;
bool prevButtonStateRight = HIGH;

void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  if (!mpuHandler.initMPU()) {
      Serial.println("MPU6050 initialization failed!");
      while (1);
  }
  Serial.println("MPU6050 initialized successfully.");

  // Set the input pins as INPUT_PULLUP
  pinMode(INPUT_PIN1, INPUT_PULLUP);
  pinMode(INPUT_PIN2, INPUT_PULLUP);
}

void loop() {
  
  float yaw, pitch, roll;
  
  // Read current button states
  int buttonStateRight = digitalRead(INPUT_PIN1);
  int buttonStateLeft = digitalRead(INPUT_PIN2);
  
  char btnLeft = '0';
  char btnRight = '0';

  // Detect a button press (transition from HIGH to LOW)
  if (buttonStateLeft == LOW && prevButtonStateLeft == HIGH) {
    btnLeft = '1';  // Register a single click
  }
  if (buttonStateRight == LOW && prevButtonStateRight == HIGH) {
    btnRight = '1';  // Register a single click
  }

  // Update the previous button states
  prevButtonStateLeft = buttonStateLeft;
  prevButtonStateRight = buttonStateRight;

  if (mpuHandler.readYPR(yaw, pitch, roll)) {
    Serial.print(yaw);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.print(roll);
    Serial.print(",");
    Serial.print(btnLeft);
    Serial.print(",");
    Serial.println(btnRight);
  }

    delay(10); // Small delay to avoid excessive CPU usage
}