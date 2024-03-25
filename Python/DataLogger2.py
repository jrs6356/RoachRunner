import serial
import struct
import threading
import os

# Global variable for the binary file
file_path = 'data.bin'
end_marker = b'\n'

# Lock for thread-safe access to the binary file
file_lock = threading.Lock()

# Clear the contents of 'data.bin' file
#if os.path.exists(file_path):
    #os.remove(file_path)

binary_file = open(file_path,'r+b')
binary_file.seek(0)
binary_file.truncate()
binary_file.close()

# Open the binary file in read-write mode
binary_file = open(file_path, 'r+b')

ser = serial.Serial('COM7', 2000000)
format_string = "<1L1f12i"
collect_data = False
new_data = False
#buffer = 0

def read_serial(serial_port):
    # Open the binary file in write-binary mode
    global collect_data
    while True:
        # Read data from the serial port
        if serial_port.inWaiting()!=0:
            line = serial_port.readline().decode().strip()
            print(line)

            # Check for start trigger
            if line == "#Start":
                # Set the flag to start collecting data
                #global collect_data
                collect_data = True
                break

def read_byte_buffer(serial_port):
    global new_data
    #buffer = b''
    while True:
        buffer = b''
        buffer += serial_port.read(2850)
        new_data = False
        while end_marker in buffer:
            packet, buffer = buffer.split(end_marker, 1)
            with file_lock:
                binary_file.seek(0,2)
                binary_file.write(packet)
                binary_file.flush()

# Function to continuously read and print data from binary file
def read_binary_file():
    global new_data
    n = 0
    while True:
        while True:
            with file_lock:
                binary_file.seek(0, 2)
                entry_size = struct.calcsize('<1L1f12i')
                previous_entry_position = binary_file.tell() - entry_size
                if previous_entry_position < 0:
                    break
                binary_file.seek(previous_entry_position)
                buffer = binary_file.read(entry_size)
                
                ulong_value, float_value, *int_values = struct.unpack('<1L1f12i', buffer)
                #print(f"{ulong_value}")
                print(f"Float: {float_value}")

read_thread = threading.Thread(target=read_byte_buffer, args=(ser,))
print_thread = threading.Thread(target=read_binary_file, args=())

read_serial(ser)


read_thread.start()
print_thread.start()

while collect_data:
    pass

read_thread.join()
ser.close()
binary_file.close()
