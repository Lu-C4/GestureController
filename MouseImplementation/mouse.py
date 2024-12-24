try:
    import serial
    import pydirectinput
    import time

    # Initialize serial communication with ESP32
    ser = serial.Serial('COM3', 115200)  # Replace 'COM3' with your ESP32 port (e.g., /dev/ttyUSB0 on Linux)

    pydirectinput.FAILSAFE = False
    # Initialize pydirectinput
    pydirectinput.PAUSE = 0  # Set to 0 for smoother movement

    # Sensitivity factor (adjust as needed)
    sensitivity = 0.2

    # Deadzone threshold to prevent jitter (adjust as needed)
    deadzone = 1.5

    def process_data(data):
        try:
            # Parse the delta x and delta y values from the sensor data
            # yaw     pitch     roll
            delta_z, delta_x, delta_y = map(float, data.split(','))

            print(delta_x, delta_y , sep=" | ")
            # If delta_x and delta_y are too small, ignore the movement (deadzone logic)
            if abs(delta_x) < deadzone and abs(delta_y) < deadzone:
                return 0, 0

            delta_x = -delta_x
            
            # Apply sensitivity to the movement
            dx = int(delta_x * sensitivity)
            dy = int(delta_y * sensitivity)

            return dx, dy
        except ValueError:
            return 0, 0

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            dx, dy = process_data(line)
            
            # Move the mouse using pydirectinput
            if dx != 0 or dy != 0:
                pydirectinput.move(dx, dy, relative=True)

        # Add a small delay to prevent excessive CPU usage
        time.sleep(0.001)

except KeyboardInterrupt:
    pass
except Exception as e:
    print(e)
finally:
    print("Closing Serial Monitor...")
    ser.close()