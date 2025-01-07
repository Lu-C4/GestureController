import tkinter as tk
from tkinter import ttk, messagebox
import serial
import threading
import pydirectinput
import time

class SerialListenerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Mouse Driver")
        
        self.root.geometry("400x200")
        self.root.resizable(False, False)
        # UI Elements
        self.com_label = ttk.Label(root, text="Select COM Port:")
        self.com_label.pack(pady=5)

        self.com_combobox = ttk.Combobox(root, values=self.get_com_ports(), state="readonly")
        self.com_combobox.pack(pady=5)

        self.connect_button = ttk.Button(root, text="Connect", command=self.connect)
        self.connect_button.pack(pady=5)

        self.stop_button = ttk.Button(root, text="Stop", command=self.stop, state="disabled")
        self.stop_button.pack(pady=5)

        # Initialize variables
        self.serial_port = None
        self.listening = False
        self.thread = None

        # Configure pydirectinput
        pydirectinput.FAILSAFE = False
        pydirectinput.PAUSE = 0
        self.sensitivity = 0.2
        self.deadzone = 1.5

    def get_com_ports(self):
        # Detect available COM ports
        import serial.tools.list_ports
        ports = serial.tools.list_ports.comports()
        return [port.device for port in ports]

    def connect(self):
        selected_port = self.com_combobox.get()
        if not selected_port:
            messagebox.showerror("Error", "Please select a COM port.")
            return

        try:
            self.serial_port = serial.Serial(selected_port, 115200)
            self.listening = True

            # Start a background thread to listen to serial data
            self.thread = threading.Thread(target=self.listen_to_serial, daemon=True)
            self.thread.start()

            self.connect_button.config(state="disabled")
            self.stop_button.config(state="normal")
            messagebox.showinfo("Connected", f"Connected to {selected_port}.")

        except Exception as e:
            messagebox.showerror("Error", f"Failed to connect: {e}")

    def listen_to_serial(self):
        def process_data(data):
            try:
                delta_z, delta_x, delta_y, clickLeft, clickRight = map(float, data.split(","))
                if abs(delta_x) < self.deadzone and abs(delta_y) < self.deadzone:
                    return 0, 0, int(clickLeft), int(clickRight)

                delta_x = -delta_x
                dx = int(delta_x * self.sensitivity)
                dy = int(delta_y * self.sensitivity)

                return dx, dy, int(clickLeft), int(clickRight)
            except ValueError:
                return 0, 0, 0, 0

        try:
            while self.listening and self.serial_port and self.serial_port.is_open:
                if self.serial_port.in_waiting > 0:
                    line = self.serial_port.readline().decode("utf-8").strip()
                    dx, dy, clickLeft, clickRight = process_data(line)

                    if dx != 0 or dy != 0:
                        pydirectinput.move(dx, dy, relative=True)

                    if clickLeft == 1:
                        pydirectinput.click(button="left")
                        time.sleep(0.1)

                    if clickRight == 1:
                        pydirectinput.click(button="right")
                        time.sleep(0.1)

                time.sleep(0.001)
        except Exception as e:
            print(f"Error in serial listener: {e}")

    def stop(self):
        self.listening = False
        if self.thread and self.thread.is_alive():
            self.thread.join(timeout=1)

        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()

        self.connect_button.config(state="normal")
        self.stop_button.config(state="disabled")
        messagebox.showinfo("Stopped", "Stopped listening to serial port.")

if __name__ == "__main__":
    root = tk.Tk()
    app = SerialListenerApp(root)
    root.mainloop()
