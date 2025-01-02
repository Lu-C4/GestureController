# Motion Classification with ESP32

This project demonstrates how to classify motion using an ESP32 with an MPU6050 sensor and a machine learning model. Follow the steps below to set up and run the project.

---

## Prerequisites
1. **Hardware Requirements**
   - ESP32 microcontroller
   - MPU6050 sensor module
   - USB cable for programming the ESP32

2. **Software Requirements**
   - Arduino IDE
   - A Bluetooth Serial Monitor application (for viewing motion classifications)

---

## Setup Instructions

### Step 1: Install a Bluetooth Serial Monitor
To view the type of motion classified by the machine learning model, install a Bluetooth Serial Monitor app on your device:

- **Android**: [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal&hl=en_IN) (Tested - Works perfectly)
- **Windows**: [Bluetooth Serial Terminal](https://apps.microsoft.com/detail/9NRS6V7H0N9B?hl=en-us&gl=IN&ocid=pdpshare) (Might not work properly)

### Step 2: Install the Motion Classifier Library
To use the motion classification functionality, install the `MotionClassifierMpu6050_inferencing.h` header file by following these steps:

1. Open the Arduino IDE.
2. Navigate to `Sketch -> Include Library -> Add .ZIP Library`.
3. Locate and select the `ei-motionclassifiermpu6050-arduino-1.0.1.zip` file.
4. After installing the library, include it in your sketch via `Sketch -> Include Library`.

### Step 3: Upload the Code to ESP32
1. Open the `Mpu6050Inferencing.ino` file in the Arduino IDE.
2. Connect your ESP32 to your computer using a USB cable.
3. Select the correct board (`ESP32`) and port from the Arduino IDE.
4. Click on the **Upload** button to burn the code into the ESP32.

### Step 4: Connect to a Bluetooth Serial Monitor
After uploading the code:
1. Power the ESP32.
2. Pair it with a device that supports a Bluetooth Serial Monitor.
3. Open the Bluetooth Serial Monitor app and connect to the ESP32.
4. Observe the motion classifications in real-time.

---

## Notes
- Ensure that the MPU6050 sensor is properly connected to the ESP32.
- If the Windows Bluetooth Serial Monitor does not function correctly, use an Android device instead.
- For any troubleshooting, refer to the documentation of the Arduino IDE and the Bluetooth Serial Monitor application.

---
