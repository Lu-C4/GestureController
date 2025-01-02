# Cursor Control with ESP32 and MPU6050

This project demonstrates how to control a computer cursor using an ESP32 with an MPU6050 sensor. Follow the steps below to set up and run the project.

---

## Setup Instructions

### Step 1: Install Requirements
Install all the required Python packages listed in `requirements.txt`.

### Step 2: Upload the Code to ESP32
Burn the `YPR.ino` code into the ESP32.

### Step 3: Configure the COM Port
Identify the COM port your ESP32 is connected to on your computer. Update the COM port in the `mouse.py` script accordingly.

### Step 4: Run the Python Script
Ensure that the serial communication is not open in any other application. Run the Python script `mouse.py` to enable cursor control.

### Step 5: Control the Cursor
Motion your hand to control the cursor accordingly.

---

## Notes
- Ensure that the MPU6050 sensor is properly connected to the ESP32.
- Close any applications that might interfere with serial communication before running the Python script.
  
---
