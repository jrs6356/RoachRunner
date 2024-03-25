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
#binary_file = open(file_path, 'r+b')

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
    buffer = b''
    while True:
        buffer += serial_port.read(4096)
        while end_marker in buffer:
            packet, buffer = buffer.split(end_marker, 1)
            
            with file_lock:
                with open(file_path, 'ab') as file:
                    file.write(packet)
                    file.flush()
                    new_data = True
                    
                #binary_file.flush()
            #new_data = True

        # Write the data to the binary file
        #if not new_data:
        #with file_lock:
            #print("Serial")
            #binary_file.write(buffer)
            #binary_file.flush()  # Flush the buffer to ensure immediate write
            #global new_data
            #new_data = True

# Function to continuously read and print data from binary file
def read_binary_file():
    global new_data
    n = 0
    while True:
        if new_data:
            with file_lock:
                with open(file_path, 'rb') as file:
                    #file.seek(n*56)
                    #n = n+1
                    buffer = file.read(56)
                    ulong_value, float_value, *int_values = struct.unpack('<1L1f12i', buffer)
                    print(f"Float: {float_value}")
                    new_data = False

            # Break the loop if no more data is available
            #if not buffer:
                #break

            # Unpack the data from the buffer

            # Print the unpacked data
            #print(f"Unsigned Long: {ulong_value}")
            #print(f"Float: {float_value}")
            #print(f"Integers:")
            #for i, value in enumerate(int_values):
            #    print(f"  Int {i+1}: {value}")
            #print()

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
