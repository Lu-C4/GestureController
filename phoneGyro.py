import pyautogui
import json
import time
from websockets.sync.client import connect

# Initialize a smoothing factor to control the smoothness
SMOOTHING_FACTOR = 0.5

# Initial positions for the low-pass filter
previous_x, previous_y = 0, 0

def move_cursor_based_on_accelerometer(x, y):
    global previous_x, previous_y
    
    # Invert the values
    x = -x
    y = -y
    
    # Apply smoothing (low-pass filter)
    x_smoothed = previous_x + (x - previous_x) * SMOOTHING_FACTOR
    y_smoothed = previous_y + (y - previous_y) * SMOOTHING_FACTOR
    
    # Update previous values for next smoothing step
    previous_x, previous_y = x_smoothed, y_smoothed

    # Scale movement to adjust sensitivity
    scaling_factor = 30
    x_movement = x_smoothed * scaling_factor
    y_movement = y_smoothed * scaling_factor

    # Get the current position of the cursor
    current_x, current_y = pyautogui.position()
    
    # Move cursor smoothly to new position
    pyautogui.moveTo(current_x + x_movement, current_y + y_movement, duration=0.05)

def main():
    with connect("ws://192.168.43.219:8080/sensor/connect?type=android.sensor.accelerometer") as websocket:
        try:
            while True:
                # Receive message from the WebSocket
                message = websocket.recv()
                
                # Parse the JSON data
                data = json.loads(message)
                values = data["values"]
                
                # Extract x, y values and ignore z for 2D movement
                x, y, _ = values
                
                # Update cursor position
                move_cursor_based_on_accelerometer(x, y)
                
                # Small delay to control update frequency
                time.sleep(0.05)
        except KeyboardInterrupt:
            print("Exiting program.")

# Run the main function
main() 
