import socket
import time
import pywifi
import struct
import numpy as np
from PyQt5.QtWidgets import QApplication, QCheckBox, QVBoxLayout, QWidget
from PyQt5.QtCore import QTimer
import pyqtgraph as pg

# WiFi details
ssid = 'RoachRunner'
password = 'RoachRunner'
UDP_IP_ADDRESS = "192.168.3.2"
UDP_PORT_NO = 2000
marco = 'Marco?'
polo = 'Polo!'
t = [0,0,0,0,0,0,0,0,0,0]
MAX_TIME = 2.6
samples = 0

# Initialize pywifi and interface
wifi = pywifi.PyWiFi()
iface = wifi.interfaces()[0]

# Initialize data and settings
n_points = 10000  # Number of points to display
y_data = [np.zeros(n_points) for _ in range(12)]
time_data = np.zeros(n_points)

# PyQTGraph setup
app = QApplication([])
win = pg.GraphicsLayoutWidget(show=True, title="Live Plotting")
pg.setConfigOptions(antialias=True)
p = win.addPlot(title="Real-time Data")
p.setXRange(0, MAX_TIME)
#p.setYRange(MIN_Y, MAX_Y)
p.setLabel('left', "ADC Value")
p.setLabel('bottom', "Time", "s")

# Create 12 curves (one for each ADC channel)
curves = [p.plot(time_data, y_data[i], pen=(i,12)) for i in range(12)]

# Set up Checkbox UI
layout = QVBoxLayout()
checkboxes = [QCheckBox(f"Channel {i}") for i in range(12)]
for checkbox in checkboxes:
    checkbox.setChecked(True)
    layout.addWidget(checkbox)

widget = QWidget()
widget.setLayout(layout)
widget.show()

# Connect to WiFi network
def connect_to_wifi(ssid, password):
    print(f"Connecting to {ssid}...")

    profile = pywifi.Profile()
    profile.ssid = ssid
    profile.auth = pywifi.const.AUTH_ALG_OPEN
    profile.akm.append(pywifi.const.AKM_TYPE_WPA2PSK)
    profile.cipher = pywifi.const.CIPHER_TYPE_CCMP
    profile.key = password

    iface.remove_all_network_profiles()
    iface.connect(iface.add_network_profile(profile))

    # Check if connected
    for _ in range(10):  # Try for 10 times with 1-second gap
        time.sleep(1)
        if iface.status() == pywifi.const.IFACE_CONNECTED:
            print(f"Successfully connected to {ssid}")
            return True
    print("Failed to connect")
    return False

# Data update function
def update():
    global y_data, time_data, samples
    data, addr = clientSock.recvfrom(320)
    
    if data:
        #print(len(data))
        format_string = "=LLf12iI"  # 4 bytes (unsigned long) + 4 bytes (unsigned long) + 4 bytes (float) + 48 bytes (12 int) + 4 bytes (int)

        # Loop through the buffer to decode each struct
        for i in range(0, len(data), 64):
            samples = samples + 1
            chunk = data[i:i+64]  # Extracting each 64-byte chunk of the buffer
            #print(len(chunk))
            decoded_data = struct.unpack(format_string, chunk)

            # Extract the fields from decoded_data tuple
            t, ti, f, *y, n = decoded_data
            new_y = y[:12]  # This should give you the list of 12 int
            new_time = f  # convert microseconds to seconds

            # Shift old data and add new data
            time_data = np.roll(time_data, -1)
            time_data[-1] = new_time
            #print(time_data[0],", ",new_time)
            fs = samples / new_time
            print(samples,", ",fs)
            if new_time > MAX_TIME:
                p.setXRange(time_data[0], new_time)

            for j in range(12):
                y_data[j] = np.roll(y_data[j], -1)
                y_data[j][-1] = new_y[j]

                # Update the plot only if checkbox is checked
                if checkboxes[j].isChecked():
                    curves[j].setData(time_data, y_data[j])

# Initialize connection
if not connect_to_wifi(ssid, password):
    print("Exiting due to connection failure.")
    exit(1)

# Create a UDP socket
clientSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
try:
    clientSock.bind((UDP_IP_ADDRESS, UDP_PORT_NO))
except OSError as e:
    print(f"Could not bind: {e}")
clientSock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 65536)


lock = False
print("Configuring data stream")
while not lock:
    data, addr = clientSock.recvfrom(1024)  # Buffer size is 1024 bytes
    data = data.decode('utf-8')
    print(f"Data Received: '{data}'")
    #print(data)
    
    if data == "Marco?":
        print(f"Received '{data}' from Arduino. Sending 'Polo!'.")
        clientSock.sendto("Polo!".encode('utf-8'), addr)
        break

print("Network Connected!")

# Update rate: every 10 ms run update()
timer = QTimer()
timer.timeout.connect(update)
timer.start(10)

# Run the application
app.exec_()
