import serial
import struct
import threading
import matplotlib.pyplot as plt

# Global variables for data storage
time_data = []
int_data = [[] for _ in range(12)]

format_string = "<1L1f12i"

# Create a lock for thread-safe access to data
data_lock = threading.Lock()

# Flag to indicate whether data collection should occur
collect_data = False

# Function to continuously read serial data and write to binary file
def read_serial(serial_port, file_path):
    # Open the binary file in write-binary mode
    with open(file_path, 'wb') as file:
        while True:
            # Read data from the serial port
            line = serial_port.readline().decode().strip()

            # Check for start trigger
            if line == "#Start":
                # Set the flag to start collecting data
                global collect_data
                collect_data = True
                break

            # Check for stop trigger
            if line == "#Stop":
                # Set the flag to stop collecting data
                collect_data = False
                continue

            # Write the data to the binary file if data collection flag is set
            if collect_data:
                # Parse and unpack the data
                buffer = line.encode()
                file.write(buffer)

# Function to continuously read byte buffers and process the data
def read_byte_buffers(serial_port, file_path):
    # Open the binary file in write-binary mode
    with open(file_path, 'wb') as file:
        while True:
            # Read data from the serial port
            buffer = serial_port.read(56)

            # Unpack the data from the buffer
            upd = struct.unpack(format_string, buffer)
            ulong_value = upd[0]
            int_values = upd[2:]

            # Acquire lock for thread-safe access to data
            with data_lock:
                # Process the data
                time_data.append(ulong_value)
                for i, value in enumerate(int_values):
                    int_data[i].append(value)

                # Write the data to the binary file
                file.write(buffer)

                # Update the plot with new data
                update_plot()

# Function to update the plot with new data
def update_plot():
    plt.clf()  # Clear the current figure
    for i, data in enumerate(int_data):
        plt.plot(time_data, data, label=f'Int {i+1}')
    plt.xlabel('Unsigned Long')
    plt.ylabel('Int Data')
    plt.legend()
    plt.grid(True)
    plt.draw()
    plt.pause(0.01)

# Open the serial port
ser = serial.Serial('COM7', 2000000)

# Specify the file path for storing the binary data
file_path = 'data.bin'

# Create a thread for reading byte buffers and processing data
buffer_thread = threading.Thread(target=read_byte_buffers, args=(ser, file_path))
buffer_thread.start()

# Create the plot
plt.figure(figsize=(10, 6))

# Continuously update the plot
while True:
    plt.pause(0.01)

# Wait for the buffer thread to complete (optional)
buffer_thread.join()

# Close the serial port
ser.close()
