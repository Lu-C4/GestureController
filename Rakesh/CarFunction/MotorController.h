#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>

class MotorController {
private:
    // Motor A connections
    int enA, in1, in2;
    // Motor B connections
    int enB, in3, in4;

    // Helper function to stop all motors
    void stopMotors() {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
    }

public:
    // Constructor to initialize motor pins
    MotorController(int enA, int in1, int in2, int enB, int in3, int in4)
        : enA(enA), in1(in1), in2(in2), enB(enB), in3(in3), in4(in4) {}

    // Initialize motor pins
    void begin(int speed = 200) {
        pinMode(enA, OUTPUT);
        pinMode(enB, OUTPUT);
        pinMode(in1, OUTPUT);
        pinMode(in2, OUTPUT);
        pinMode(in3, OUTPUT);
        pinMode(in4, OUTPUT);

        // Set initial speed
        analogWrite(enA, speed);
        analogWrite(enB, speed);

        // Stop all motors initially
        stopMotors();
    }

    // Update the motor state based on the input command
    void updateState(char command) {
        stopMotors(); // Ensure motors are stopped before changing state
        switch (command) {
            case 'F': // Forward
                digitalWrite(in1, HIGH);
                digitalWrite(in2, LOW);
                digitalWrite(in3, HIGH);
                digitalWrite(in4, LOW);
                break;

            case 'B': // Backward
                digitalWrite(in1, LOW);
                digitalWrite(in2, HIGH);
                digitalWrite(in3, LOW);
                digitalWrite(in4, HIGH);
                break;

            case 'L': // Left
                digitalWrite(in1, LOW);
                digitalWrite(in2, LOW);
                digitalWrite(in3, HIGH);
                digitalWrite(in4, LOW);
                break;

            case 'R': // Right
                digitalWrite(in1, HIGH);
                digitalWrite(in2, LOW);
                digitalWrite(in3, LOW);
                digitalWrite(in4, LOW);
                break;

            case 'X': // Backward Left
                digitalWrite(in1, LOW);
                digitalWrite(in2, LOW);
                digitalWrite(in3, LOW);
                digitalWrite(in4, HIGH);
                break;

            case 'Y': // Backward Right
                digitalWrite(in1, LOW);
                digitalWrite(in2, HIGH);
                digitalWrite(in3, LOW);
                digitalWrite(in4, LOW);
                break;

            case 'S': // Still
            default:
                stopMotors();
                break;
        }
    }
};

#endif
