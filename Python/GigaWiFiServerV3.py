import socket
import sys
import time
import pywifi
import struct
import numpy as np
from PyQt5.QtWidgets import QMainWindow, QApplication, QCheckBox, QVBoxLayout, QGridLayout, QWidget
from PyQt5.QtCore import QTimer, QThread, Qt
from PyQt5.QtGui import QColor, QPen
from pyqtgraph import PlotWidget, plot, mkPen
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
BUFFER_SIZE = 5000
ARRAY_SIZE = 100
q = [Queue(maxsize=0) for _ in range(12)]
#y_dat = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y0 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y1 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y2 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y3 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y4 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y5 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y6 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y7 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y8 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y9 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y10 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)
y11 = deque([0.0]*BUFFER_SIZE, maxlen=BUFFER_SIZE)

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
win = MyMainWindow()
central_widget = QWidget()
win.setCentralWidget(central_widget)
layout = QGridLayout()
plt = PlotWidget()
layout.addWidget(plt,0,0,1,1)
plt.setYRange(-15,15)
central_widget.setLayout(layout)
win.show()
#curve = plt.plot(y_dat)
c0 = plt.plot(y0)
c1 = plt.plot(y1)
c2 = plt.plot(y2)
c3 = plt.plot(y3)
c4 = plt.plot(y4)
c5 = plt.plot(y5)
c6 = plt.plot(y6)
c7 = plt.plot(y7)
c8 = plt.plot(y8)
c9 = plt.plot(y9)
c10 = plt.plot(y10)
c11 = plt.plot(y11)
c0.setPen(QPen(QColor("red")))
c1.setPen(QPen(QColor("blue")))
c2.setPen(QPen(QColor("green")))
c3.setPen(QPen(QColor("white")))
c4.setPen(QPen(QColor("darkYellow")))
c5.setPen(QPen(QColor("cyan")))
c6.setPen(QPen(QColor("magenta")))
c7.setPen(QPen(QColor("orange")))
c8.setPen(QPen(QColor("darkGreen")))
c9.setPen(QPen(QColor("gray")))
c10.setPen(QPen(QColor("darkMagenta")))
c11.setPen(QPen(QColor("darkBlue")))

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
    new0 = []
    new1 = []
    new2 = []
    new3 = []
    new4 = []
    new5 = []
    new6 = []
    new7 = []
    new8 = []
    new9 = []
    new10 = []
    new11 = []
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
                
                #new_dat.append(y[6])
                new0.append(y[0])
                new1.append(y[1])
                new2.append(y[2])
                new3.append(y[3])
                new4.append(y[4])
                new5.append(y[5])
                new6.append(y[6])
                new7.append(y[7])
                new8.append(y[8])
                new9.append(y[9])
                new10.append(y[10])
                new11.append(y[11])
                
                idx = idx + 1
                if idx >= ARRAY_SIZE:
                    q[0].put(new0)
                    q[1].put(new1)
                    q[2].put(new2)
                    q[3].put(new3)
                    q[4].put(new4)
                    q[5].put(new5)
                    q[6].put(new6)
                    q[7].put(new7)
                    q[8].put(new8)
                    q[9].put(new9)
                    q[10].put(new10)
                    q[11].put(new11)
                    idx = 0
                    new0 = []
                    new1 = []
                    new2 = []
                    new3 = []
                    new4 = []
                    new5 = []
                    new6 = []
                    new7 = []
                    new8 = []
                    new9 = []
                    new10 = []
                    new11 = []

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

def update():
    if not q[0].empty():
        new0 = q[0].get()
        new1 = q[1].get()
        new2 = q[2].get()
        new3 = q[3].get()
        new4 = q[4].get()
        new5 = q[5].get()
        new6 = q[6].get()
        new7 = q[7].get()
        new8 = q[8].get()
        new9 = q[9].get()
        new10 = q[10].get()
        new11 = q[11].get()
        for point in new0:
            y0.append(point)
        for point in new1:
            y1.append(point)
        for point in new2:
            y2.append(point)
        for point in new3:
            y3.append(point)
        for point in new4:
            y4.append(point)
        for point in new5:
            y5.append(point)
        for point in new6:
            y6.append(point)
        for point in new7:
            y7.append(point)
        for point in new8:
            y8.append(point)
        for point in new9:
            y9.append(point)
        for point in new10:
            y10.append(point)
        for point in new11:
            y11.append(point)
        
        c0.setData(y0)
        c1.setData(y1)
        c2.setData(y2)
        c3.setData(y3)
        c4.setData(y4)
        c5.setData(y5)
        c6.setData(y6)
        c7.setData(y7)
        c8.setData(y8)
        c9.setData(y9)
        c10.setData(y10)
        c11.setData(y11)

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
