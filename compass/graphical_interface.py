import serial
import tkinter as tk
from math import sin, cos, radians

# Replace 'COM3' with the correct serial port for your Arduino
ser = serial.Serial('COM4', 9600, timeout=1)

class CompassApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Arduino Compass Interface")

        # Create canvas with dark blue circle
        self.canvas = tk.Canvas(root, width=400, height=400, bg="dark blue")
        self.canvas.create_oval(50, 50, 350, 350, fill="dark blue", outline="white")
        self.canvas.pack()

        # Place the beginning of the arrow at the center of the circle
        self.arrow = self.canvas.create_line(200, 200, 200, 100, arrow=tk.LAST, width=2, fill="red")

        # Add labels for all directions at the edge of the circle
        self.direction_labels = []
        label_distance = 150  # Initial distance
        for angle, direction in zip(range(0, 360, 45), ["N", "NE", "E", "SE", "S", "SW", "W", "NW"]):
            # Adjust label distance dynamically based on label text
            label_width = len(direction) * 6  # Assuming average character width is 6 pixels
            adjusted_distance = label_distance + label_width
            x = 200 + adjusted_distance * cos(radians(angle))
            y = 200 - adjusted_distance * sin(radians(angle))
            label = tk.Label(root, text=direction, fg="white", bg="dark blue")
            label.place(x=x, y=y, anchor="center")
            self.direction_labels.append(label)

        self.update_compass()
    def get_direction(self, azimuth):
        # Define cardinal directions
        directions = ["N", "NE", "E", "SE", "S", "SW", "W", "NW", "N"]

        # Calculate the index based on the azimuth
        index = int((azimuth + 22.5) % 360 // 45)

        # Return the corresponding cardinal direction
        return directions[index]
    def update_compass(self):
        try:
            data = ser.readline().decode().strip()
            print(data)  # For debugging

            # Check if the received data starts with 'Azimuth:'
            if data.startswith('Azimuth:'):
                azimuth_str = data.split(":")[1].strip()
            
            # Check if the azimuth_str is a numeric value before converting
                if azimuth_str.replace("-","").isdigit():
                    azimuth = int(azimuth_str)
                # Rest of your code to update the arrow direction based on azimuth
                    direction = self.get_direction(azimuth)
                    angle = azimuth
                    x1, y1, x2, y2 = self.canvas.coords(self.arrow)
                    length = 100
                    x2 = x1 + length * cos(radians(angle))
                    y2 = y1 - length * sin(radians(angle))
                    self.canvas.coords(self.arrow, x1, y1, x2, y2)
                    

                else:
                    print("Invalid azimuth value:", azimuth_str)
            else:
                print("Invalid data format:", data)

        except ValueError as e:
            print("Invalid data received:", e)
            print("Received data:", data)
    
        self.root.after(1000, self.update_compass)
if __name__ == "__main__":
    root = tk.Tk()
    app = CompassApp(root)
    root.mainloop()
