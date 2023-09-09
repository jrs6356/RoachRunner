import socket
import sys
import time
import pywifi
import struct
import numpy as np
from PyQt5.QtWidgets import QMainWindow, QApplication, QCheckBox, QVBoxLayout, QGridLayout, QWidget
from PyQt5.QtCore import QTimer, QThread
from pyqtgraph import PlotWidget, plot
import pyqtgraph as pg
from collections import deque
from queue import Queue
from threading import Thread

# WiFi details
ssid = 'RoachRunner'
password = 'RoachRunner'
UDP_IP_ADDRESS = "192.168.3.2"
UDP_PORT_NO = 2000
marco = 'Marco?'
polo = 'Polo!'
#t = [0,0,0,0,0,0,0,0,0,0]
MAX_TIME = 2.6
samples = 0

# Initialize pywifi and interface
wifi = pywifi.PyWiFi()
iface = wifi.interfaces()[0]

# Create a Queue
BUFFER_SIZE = 10000
ARRAY_SIZE = 100
q = Queue(maxsize=0)
y_dat = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)

# Initialize data and settings
n_points = 10000  # Number of points to display
y_data = [np.zeros(n_points) for _ in range(12)]
time_data = np.zeros(n_points)
t1 = time.time()
t2 = time.time()

# PyQTGraph setup
fun_alive = True
class MyMainWindow(QMainWindow):
    def __init__(self):
        super(MyMainWindow, self).__init__()

    def closeEvent(self,event):
        global fun_alive
        fun_alive = False
        print("Kill all functions")
        clientSock.sendto("Goodbye".encode('utf-8'), addr)
        event.accept()

app = QApplication(sys.argv)
#win = QMainWindow()
win = MyMainWindow()
central_widget = QWidget()
win.setCentralWidget(central_widget)
layout = QGridLayout()
plt = PlotWidget()
layout.addWidget(plt,0,0,1,1)
plt.setYRange(0,15)
central_widget.setLayout(layout)
#win.destroyed.connect(buzzkill)
win.show()
#y_dat = []
curve = plt.plot(y_dat)

#win = pg.GraphicsLayoutWidget(show=True, title="Live Plotting")
#pg.setConfigOptions(antialias=True)
#p = win.addPlot(title="Real-time Data")
#p.setXRange(0, MAX_TIME)
#p.setYRange(MIN_Y, MAX_Y)
#p.setLabel('left', "ADC Value")
#p.setLabel('bottom', "Time", "s")

# Create 12 curves (one for each ADC channel)
#curves = [p.plot(time_data, y_data[i], pen=(i,12)) for i in range(12)]

# Set up Checkbox UI
#layout = QVBoxLayout()
#checkboxes = [QCheckBox(f"Channel {i}") for i in range(12)]
#for checkbox in checkboxes:
    #checkbox.setChecked(True)
    #layout.addWidget(checkbox)

#widget = QWidget()
#widget.setLayout(layout)
#widget.show()

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
def read_dat(q):
    global samples, t1, t2, fun_alive
    format_string = "=LLf12fI"
    idx = 0
    new_dat = []
    t1 = time.time()
    while fun_alive:
        data, addr = clientSock.recvfrom(320)
        if data:
            for i in range(0, len(data), 64):
                samples = samples + 1
                t2 = time.time()
                chunk = data[i:i+64]
                decoded_data = struct.unpack(format_string, chunk)
                t, ti, f, *y, n = decoded_data
                new_dat.append(y[6])
                idx = idx + 1
                if idx >= ARRAY_SIZE:
                    q.put(new_dat)
                    idx = 0
                    new_dat = []

def calc_freq(q):
    global samples, t1, t2, fun_alive
    samples = 0
    t2 = time.time()
    t1 = time.time()
    while fun_alive:
        if t2 > t1:
            dt = t2 - t1
            fs = samples/dt
            print(samples,", ",fs)
        time.sleep(1)
    
def read_dat_sloppy(q):
    global y_data, time_data, samples
    format_string = "=LLf12fI"  # 4 bytes (unsigned long) + 4 bytes (unsigned long) + 4 bytes (float) + 48 bytes (12 int) + 4 bytes (int)
    idx = 0
    new_dat = []
    t1 = time.time()
    
    while True:
        data, addr = clientSock.recvfrom(320)
        
        if data:
            # Loop through the buffer to decode each struct
            for i in range(0, len(data), 64):
                samples = samples + 1
                chunk = data[i:i+64]  # Extracting each 64-byte chunk of the buffer
                #print(len(chunk))
                decoded_data = struct.unpack(format_string, chunk)

                # Extract the fields from decoded_data tuple
                t, ti, f, *y, n = decoded_data
                #new_y = y[:12]  # This should give you the list of 12 int
                #new_time = f  # convert microseconds to seconds
                new_dat.append(y[6])
                idx = idx + 1
                if idx >= ARRAY_SIZE:
                    q.put(new_dat)
                    idx = 0
                    new_dat = []

                # Shift old data and add new data
                #time_data = np.roll(time_data, -1)
                #time_data[-1] = new_time
                #print(time_data[0],", ",new_time)
                fs = samples / (time.time() - t1)
                print(samples,", ",fs)
                #if new_time > MAX_TIME:
                    #p.setXRange(time_data[0], new_time)

                #for j in range(12):
                    #y_data[j] = np.roll(y_data[j], -1)
                    #y_data[j][-1] = new_y[j]

                    # Update the plot only if checkbox is checked
                    #if checkboxes[j].isChecked():
                        #curves[j].setData(time_data, y_data[j])

def update():
    if not q.empty():
        new_data = q.get()
        for point in new_data:
            y_dat.append(point)
        curve.setData(y_dat)
        curve.setData(y_dat)

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
timer.start(1)

read_thread = Thread(target=read_dat,args=(q,))
read_thread.daemon = True
read_thread.start()

#timer2 = QTimer()
#timer2.timeout.connect(calc_freq)
#timer2.start(100)

freq_thread = Thread(target=calc_freq,args=(q,))
freq_thread.daemon = True
freq_thread.start()

# Run the application
sys.exit(app.exec_())
