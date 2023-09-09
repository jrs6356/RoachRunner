import serial 
import argparse
import sys
import os
import tkinter as tk
from tkinter import ttk, Tk, Label, Button, Frame
from PIL import Image, ImageTk
import PIL
import csv
import pandas as pd
import numpy as np
import math
import statistics as st
import time
from matplotlib import pyplot as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from mpl_toolkits.mplot3d import Axes3D
from PyQt5 import QtWidgets, QtCore
from pyqtgraph import PlotWidget, plot
import pyqtgraph as pg
from collections import deque
from scipy.ndimage import shift
import array as arr

def mainHandler():
    forcePlotter2()

def forcePlotter():
    port = 'COM6'
    serial_port = serial.Serial(port,250000)
    global status
    status = True

    global T, X, Y, Z
    global count
    global clean
    count = 0
    clean = True
    T = np.zeros(100)
    X = np.zeros(100)
    Y = np.zeros(100)
    Z = np.zeros(100)
    U = 0.0
    V = 0.0
    W = 0.0
    
    plt.ion()
    fig = plt.figure()
    
                                            #ax = fig.add_subplot(111, projection='3d')
                                            #ax = fig.gca(projection='3d')
                                            #ax.set_xlim([-10, 10])
                                            #ax.set_ylim([-10, 10])
                                            #ax.set_zlim([-10, 10])
                                            #segs = [0.0,0.0,0.0,U,V,W]
                                            #acc = ax.quiver(*segs)
    
    xax = fig.add_subplot(311)
    yax = fig.add_subplot(312)
    zax = fig.add_subplot(313)
    xax.set_ylim([-1, 1])
    yax.set_ylim([-1, 1])
    zax.set_ylim([-1, 1])
    xax.set_xlim([0, 10])
    yax.set_xlim([0, 10])
    zax.set_xlim([0, 10])
    xacc, = xax.plot(T, X)
    yacc, = yax.plot(T, Y)
    zacc, = zax.plot(T, Z)

    plt.show()
    time.sleep(2)

    def setup():
        print('Setup')
        #portConnect()

    def main():
        print('Main')
        global status
        global count
        global clean
        while True:
            if status:
                if serial_port.inWaiting()==0:
                    #time.sleep(.0001)
                    pass
                else:
                    try:
                        if clean:
                            line = serial_port.readline().decode().strip()
                            print(line)
                            if line=="#Start":
                                clean = False
                        else:
                            if count<10:
                                #print(count)
                                count = count + 1
                                line = serial_port.readline().decode().strip()
                            else:
                                line = serial_port.readline().decode().strip()
                                t = float((line.partition(',')[0]))
                                x = float(line.partition(',')[2].partition(',')[0])
                                y = float(line.partition(',')[2].partition(',')[2].partition(',')[0])
                                z = float((line.partition(',')[2].partition(',')[2].partition(',')[2]))
                                print(t," , ",x," , ",y," , ",z)
                                
                                T_new = np.roll(T,-1)
                                X_new = np.roll(X,-1)
                                Y_new = np.roll(Y,-1)
                                Z_new = np.roll(Z,-1)
                                
                                T_new[99] = t
                                X_new[99] = x
                                Y_new[99] = y
                                Z_new[99] = z
                                
                                T[:] = arr.array('f',T_new)
                                X[:] = arr.array('f',X_new)
                                Y[:] = arr.array('f',Y_new)
                                Z[:] = arr.array('f',Z_new)
##
##                                            #if count==100:
##                                                #print("--------------------------------")
##                                                #for i in range(0,100):
##                                                    #print(T[i],'\t',X[i])
##                                            #elif count<100:
##                                                #print(count)
##                                                #print(line.strip())
##                                
                                xacc.set_ydata(X)
                                yacc.set_ydata(Y)
                                zacc.set_ydata(Z)

                                xacc.set_xdata(T)
                                yacc.set_xdata(T)
                                zacc.set_xdata(T)

                                xax.set_xlim([T[0], T[99]+1])
                                yax.set_xlim([T[0], T[99]+1])
                                zax.set_xlim([T[0], T[99]+1])

                                xax.set_ylim([min(X), max(X)])
                                yax.set_ylim([min(Y), max(Y)])
                                zax.set_ylim([min(Z), max(Z)])
                                
                                plt.draw()
                                plt.pause(0.01)

                                #plotx.setData(T,X)
                                #ploty.setData(T,Y)
                                #plotz.setData(T,Z)
                        
                    except:
                        pass

    def portConnect():
        print('Port Connect')
        global status
        status = not status
        print(status)
        if status:
            serial_port.open()
            status = True
        else:
            serial_port.close()
            status = False

    #def plot3d(U, V, W):
        #soa = np.array([[0, 0, 1, 1, -2, 0], [0, 0, 2, 1, 1, 0],
        #                [0, 0, 3, 2, 1, 0], [0, 0, 4, 0.5, 0.7, 0]])
        #X, Y, Z, U, V, W = zip(*soa)
        #fig = plt.figure()
        #ax = fig.add_subplot(111, projection='3d')
        #ax.quiver(X, Y, Z, U, V, W)
        #ax.set_xlim([-1, 0.5])
        #ax.set_ylim([-1, 1.5])
        #ax.set_zlim([-1, 8])
        #plt.show()

    setup()
    main()

def forcePlotter2():
    port = 'COM6'
    serial_port = serial.Serial(port,250000)
    global status
    status = True

    #global T, X, Y, Z
    global count
    global clean
    count = 0
    clean = True
    #T = np.zeros(100)
    #X = np.zeros(100)
    #Y = np.zeros(100)
    #Z = np.zeros(100)
    
##    xax = fig.add_subplot(311)
##    yax = fig.add_subplot(312)
##    zax = fig.add_subplot(313)
##    xax.set_ylim([-1, 1])
##    yax.set_ylim([-1, 1])
##    zax.set_ylim([-1, 1])
##    xax.set_xlim([0, 10])
##    yax.set_xlim([0, 10])
##    zax.set_xlim([0, 10])
##    xacc, = xax.plot(T, X)
##    yacc, = yax.plot(T, Y)
##    zacc, = zax.plot(T, Z)

    #time.sleep(2)
    def cleanUp():
        print("Clean Up")
        global clean
        while clean:
            if serial_port.inWaiting()==0:
                #time.sleep(.0001)
                pass
            else:
                try:
                    line = serial_port.readline().decode().strip()
                    print(line)
                    if line=="#Start":
                        clean = False
                except:
                    pass
    
    class MainWindow(QtWidgets.QMainWindow):
        def __init__(self, *args, **kwargs):
            super(MainWindow, self).__init__(*args, **kwargs)

            self.win = pg.GraphicsLayoutWidget()
            self.setCentralWidget(self.win)

            self.k = 0
            self.xo = 0
            self.yo = 0
            self.zo = 0
            self.axo = 0
            self.ayo = 0
            self.azo = 0
            self.stax = 0
            self.stay = 0
            self.staz = 0
            self.dwsp = 1
            self.dwspk = 0

            #self.T = np.zeros(1000)
            self.T = np.linspace(0,1,1000)
            self.X = np.zeros(1000)
            #self.X = np.linspace(0,10000,1000)
            self.Y = np.zeros(1000)
            self.Z = np.zeros(1000)
            
            self.AX = np.zeros(1000)
            #self.AX = np.linspace(10000,0,1000)
            self.AY = np.zeros(1000)
            self.AZ = np.zeros(1000)

            self.win.setBackground('w')
            pen = pg.mkPen(color=(100,100,255))
            pen2 = pg.mkPen(color=(255,100,100))
            
            #win = pg.GraphicsLayoutWidget()
            self.plx = self.win.addPlot(row=0,col=0)
            self.ply = self.win.addPlot(row=1,col=0)
            self.plz = self.win.addPlot(row=2,col=0)

            #self.plotx = self.plx.plot(self.T,self.X,pen=pen)
            #self.ploty = self.ply.plot(self.T,self.Y,pen=pen)
            #self.plotz = self.plz.plot(self.T,self.Z,pen=pen)

            self.multplotx = self.plx.multiDataPlot(x=self.T,y=[self.X,self.AX],pen=[pen,pen2])
            self.multploty = self.ply.multiDataPlot(x=self.T,y=[self.Y,self.AY],pen=[pen,pen2])
            self.multplotz = self.plz.multiDataPlot(x=self.T,y=[self.Z,self.AZ],pen=[pen,pen2])
            self.plotx = self.multplotx[0]
            self.plotax = self.multplotx[1]
            self.ploty = self.multploty[0]
            self.plotay = self.multploty[1]
            self.plotz = self.multplotz[0]
            self.plotaz = self.multplotz[1]
            #self.ploty = self.ply.plot(self.T,self.Y,pen=pen)
            #self.plotz = self.plz.plot(self.T,self.Z,pen=pen)

            self.plx.setLabel('left',text='X (LSB)')
            self.ply.setLabel('left',text='Y (LSB)')
            self.plz.setLabel('left',text='Z (LSB)')
            self.plx.setLabel('bottom',text='t (s)')
            self.ply.setLabel('bottom',text='t (s)')
            self.plz.setLabel('bottom',text='t (s)')

            #self.plx.setDownsampling(ds=5)
            #self.ply.setDownsampling(ds=5)
            #self.plz.setDownsampling(ds=5)

            self.timer = QtCore.QTimer()
            self.timer.setInterval(0)
            self.timer.timeout.connect(self.update_plot_data)
            self.timer.start()

        def update_plot_data(self):
            #print("Update")
            if serial_port.inWaiting()==0:
                #time.sleep(.001)
                pass
            else:
                try:
                    self.probe()
                except:
                    pass
        def probe(self):
            line = serial_port.readline().decode().strip().split(',')
            if(self.dwspk<self.dwsp):
                self.dwspk = self.dwspk+1
            else:
                self.dwspk = 0
                t = float(line[0])
                x = -1.0*float(line[1])
                y = -1.0*float(line[2])
                z = float(line[3])
                ax = float(line[4])
                ay = float(line[5])
                az = float(line[6])
                #t = float((line.partition(',')[0]))
                #x = float(line.partition(',')[2].partition(',')[0])
                #y = float(line.partition(',')[2].partition(',')[2].partition(',')[0])
                #z = float((line.partition(',')[2].partition(',')[2].partition(',')[2]))
                #print(t,",",x,",",y,",",z,",",ax,",",ay,",",az)
                
                T_new = np.roll(self.T,-1)
                X_new = np.roll(self.X,-1)
                Y_new = np.roll(self.Y,-1)
                Z_new = np.roll(self.Z,-1)
                AX_new = np.roll(self.AX,-1)
                AY_new = np.roll(self.AY,-1)
                AZ_new = np.roll(self.AZ,-1)

                #print(t,T_new[998],5*T_new[998])
                if(t>T_new[998]) and (t<5*(T_new[998]+.01)):
                    #print("sup")
                    if(self.k<100):
                        self.k = self.k+1
                        T_new[999] = t
                        X_new[999] = x
                        Y_new[999] = y
                        Z_new[999] = z
                        AX_new[999] = ax
                        AY_new[999] = ay
                        AZ_new[999] = az

                    elif(self.k==100):
        
                        self.k = self.k+1
                        self.xo = st.mean(X_new)
                        self.yo = st.mean(Y_new)
                        self.zo = st.mean(Z_new)
                        self.axo = st.mean(AX_new)
                        self.ayo = st.mean(AY_new)
                        #self.azo = st.mean(AZ_new)
                        T_new[999] = t
                        X_new[999] = x - self.xo
                        Y_new[999] = y - self.yo
                        Z_new[999] = z - self.zo + 9.81
                        AX_new[999] = ax - self.axo
                        AY_new[999] = ay - self.ayo
                        AZ_new[999] = az
                        #self.stax = 5.0*st.stdev(AX_new)
                        #self.stay = 5.0*st.stdev(AY_new)
                        #self.staz = 5.0*st.stdev(AZ_new)

                    else:
                        T_new[999] = t
                        if(abs(x/st.mean(X_new))>100):
                            X_new[999] = X_new[998]
                        else:
                            X_new[999] = x - self.xo
                        if(abs(y/st.mean(Y_new))>100):
                            Y_new[999] = Y_new[998]
                        else:
                            Y_new[999] = y - self.yo
                        if(abs(z/st.mean(Z_new))>50):
                            Z_new[999] = Z_new[998]
                        else:
                            Z_new[999] = z + 9.81
                        
                        if(abs(ax/st.mean(AX_new))>100):
                            AX_new[999] = AX_new[998]
                        else:
                            AX_new[999] = ax
                        if(abs(ay/st.mean(AY_new))>100):
                            AY_new[999] = AY_new[998]
                        else:
                            AY_new[999] = ay
                        if(abs(az/st.mean(AZ_new))>10):
                            AZ_new[999] = AZ_new[998]
                        else:
                            AZ_new[999] = az
                        
                    self.T[:] = arr.array('f',T_new)
                    self.X[:] = arr.array('f',X_new)
                    self.Y[:] = arr.array('f',Y_new)
                    self.Z[:] = arr.array('f',Z_new)
                    self.AX[:] = arr.array('f',AX_new)
                    self.AY[:] = arr.array('f',AY_new)
                    self.AZ[:] = arr.array('f',AZ_new)

                    #print(self.T[999],",",self.X[999],",",self.Y[999],",",self.Z[999],",",self.AX[999])

                    self.plotx.setData(self.T, self.X)
                    self.plotax.setData(self.T, self.AX)
                    self.ploty.setData(self.T, self.Y)
                    self.plotay.setData(self.T, self.AY)
                    self.plotz.setData(self.T, self.Z)
                    self.plotaz.setData(self.T, self.AZ)

    
    cleanUp()
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec_())
    #while True:
        #w.update_plot_data()
    
mainHandler()
