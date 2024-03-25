import bluetooth

# Define the UUIDs for the service and characteristic
service_uuid = "19B10000-E8F2-537E-4F6C-D104768A1214"
characteristic_uuid = "19B10001-E8F2-537E-4F6C-D104768A1214"

# Define the MAC address of the Arduino GIGA R1
device_address = "a8:61:0a:28:62:73"  # Replace with the actual MAC address

def handle_data(data):
    # This function will be called when data is received
    print("Received data:", data)

# Create a Bluetooth socket
sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)

try:
    # Connect to the Bluetooth peripheral
    sock.connect((device_address, 1))
    
    while True:
        # Receive data from the peripheral
        data = sock.recv(1024).decode("utf-8")
        handle_data(data)
        
finally:
    # Clean up
    sock.close()
