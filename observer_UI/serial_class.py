import serial
import serial.tools.list_ports

import re

class DataParser:
    def __init__(self):
        pass

    def parse_data(self,received_data:str=""):
        pattern = r'x:\s*(\d+)\s*y:\s*(\d+)\s*state:\s*(\d+)\s*received_ID:\s*(\d+)\s*target x:\s*(\d+)\s*target y:\s*(\d+)'
        match = re.search(pattern, received_data)
        if match:
            return {
                'x': int(match.group(1)),
                'y': int(match.group(2)),
                'state': int(match.group(3)),
                'received_ID': int(match.group(4)),
                'target_x': int(match.group(5)),
                'target_y': int(match.group(6)),
            }
        else:
            return None
        
class SerialPortReader:
    def __init__(self, port, baud_rate=9600, timeout=1):
        """
        Initialize the serial port reader.

        :param port: The serial port to read from (e.g., 'COM3' on Windows or '/dev/ttyUSB0' on Linux).
        :param baud_rate: The baud rate for the serial communication (default is 9600).
        :param timeout: The read timeout in seconds (default is 1 second).
        """
        self.port = port
        self.baud_rate = baud_rate
        self.timeout = timeout
        self.serial_connection = None

    def print_available_ports(self):
        """
        Get a list of available serial ports.

        :return: A list of available serial ports.
        """
        ports = serial.tools.list_ports.comports()
        for port in ports:
            print(port.name)
    
    def set_port(self, port):
        """
        Set the serial port to read from.

        :param port: The serial port to read from (e.g., 'COM3' on Windows or '/dev/ttyUSB0' on Linux).
        """
        self.port = port

    def open(self):
        """
        Open the serial port connection.
        """
        try:
            self.serial_connection = serial.Serial(self.port, self.baud_rate, timeout=self.timeout)
            print(f"Serial port {self.port} opened successfully.")
        except serial.SerialException as e:
            print(f"Error opening serial port {self.port}: {e}")

    def close(self):
        """
        Close the serial port connection.
        """
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.close()
            print(f"Serial port {self.port} closed.")

    def read_line(self):
        """
        Read a line of data from the serial port.

        :return: The read line as a string, or None if no data is available.
        """
        if self.serial_connection and self.serial_connection.is_open:
            try:
                line = self.serial_connection.readline().decode('utf-8').strip()
                return line
            except serial.SerialException as e:
                print(f"Error reading from serial port {self.port}: {e}")
                return None
        else:
            print(f"Serial port {self.port} is not open.")
            return None

    def read_all(self):
            """
            Read all available data from the serial port.

            :return: The read data as a string, or None if no data is available.
            """
            if self.serial_connection and self.serial_connection.is_open:
                try:
                    data = self.serial_connection.read_all().decode('utf-8')
                    return data
                except serial.SerialException as e:
                    print(f"Error reading from serial port {self.port}: {e}")
                    return None
            else:
                print(f"Serial port {self.port} is not open.")
                return None
