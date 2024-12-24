#ifndef MPU6050_ESP32_H
#define MPU6050_ESP32_H

#include <Wire.h>
#include "MPU6050_6Axis_MotionApps20.h"

// MPU6050 instance
class MPU6050Handler {
private:
    MPU6050 mpu;
    bool dmpReady = false;
    uint8_t devStatus;
    uint16_t packetSize;
    uint16_t fifoCount;
    uint8_t fifoBuffer[64];
    VectorFloat gravity;
    Quaternion q;
    float ypr[3];

public:
    MPU6050Handler() : mpu() {}

    bool initMPU() {
        Wire.begin(); // Initialize I2C for ESP32
        Serial.println("Initializing MPU6050...");
        mpu.initialize();

        if (!mpu.testConnection()) {
            Serial.println("MPU6050 connection failed!");
            return false;
        }
        Serial.println("MPU6050 connection successful!");

        // Initialize DMP
        devStatus = mpu.dmpInitialize();

        if (devStatus == 0) {
            Serial.println("Enabling DMP...");
            mpu.setDMPEnabled(true);
            mpu.resetFIFO();  // Reset FIFO here
            dmpReady = true;
            packetSize = mpu.dmpGetFIFOPacketSize();
            Serial.println("DMP ready!");
        } else {
            Serial.print("DMP Initialization failed (code ");
            Serial.print(devStatus);
            Serial.println(")");
            return false;
        }

        return true;
    }

    bool isDMPReady() {
        return dmpReady;
    }

    bool readYPR(float &yaw, float &pitch, float &roll) {
        if (!dmpReady) return false;

        fifoCount = mpu.getFIFOCount();

        // Check for overflow
        if ((mpu.getIntStatus() & 0x10) || fifoCount == 1024) {
            mpu.resetFIFO();
            Serial.println("FIFO overflow!");
            return false;
        }

        // Ensure enough data is available
        if (fifoCount < packetSize) return false;

        // Read data
        while (fifoCount >= packetSize) {
            mpu.getFIFOBytes(fifoBuffer, packetSize);
            fifoCount -= packetSize;
        }

        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        yaw = ypr[0] * 180 / M_PI;
        pitch = ypr[1] * 180 / M_PI;
        roll = ypr[2] * 180 / M_PI;

        return true;
    }
};

#endif // MPU6050_ESP32_H
