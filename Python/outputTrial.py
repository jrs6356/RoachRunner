import serial 
import argparse
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

def mainHandler():
    serial_monitor()
    #testPlotter()

def testPlotter():
    print('plot')

def serial_monitor():
    window = tk.Tk()

    #--------------- Page Settings---------------------------------------------------------------------------

    port = 'COM6'
    #port = 'COM7'#(Arduino Due(Native USB Port))'
    #serial_port = serial.Serial(port,115200)
    serial_port = serial.Serial(port,250000)

    color = '#856ff8'
    LARGEFONT = 'Helvetica 20 bold'
    MEDFONT = 'Helvetica 14 bold'
    BUTTONFONT = 'Calibri 10'
    buh = 20
    buw = 100
    h = 1080-72
    #w = (1920/2)-1
    w = 1920-1
    x = -7
    y = 0
    #border = 'flat'
    border = 'ridge'
    bw = 1
    window.geometry('%dx%d+%d+%d' % (w, h, x, y))
    
    pixel = tk.PhotoImage(width=1, height=1)
    img = Image.open('Indicator_On.png')
    onInd = ImageTk.PhotoImage(img.resize((10,10)))
    img = Image.open('Indicator_Off.png')
    offInd = ImageTk.PhotoImage(img.resize((10,10)))

    serFile = open('SerialData.txt', mode='r+')
    serFile.truncate(0)
    serCSV = open('SerialData.csv', mode='r+', newline='')
    serCSV.truncate(0)
    serCSVWriter = csv.writer(serCSV)
    strainCSV = open('StrainCalData.csv', mode='r+', newline='')
    strainCSV.truncate(0)
    df = pd.DataFrame()
    dfCal = pd.DataFrame()
    v1 = []
    v2 = []
    v3 = []
    v4 = []
    v5 = []

    #-----Beam-----
    g = 9.81
    b = 0.0153
    h = 0.0007
    c = h/2.0
    xf = 0.0448
    xs = 0.00275
    E = 68.9*pow(10,9)
    Ix = b*pow(h,3)/12.0
    #-----Signal Conditioning-----
    Rnom = 350.0
    Vs = 5.0
    S = 1.9
    Gia = 100.0
    #-----ADC-----
    Gadc = 32.0
    A = 2.0
    Vref = 5.0
    N = 24.0
    Kbi = 2.0
    RgD = 25326871
    RoD = 3906343680

    global Vfs
    Vfs = 0.0
    global Vspan
    Vspan = 0.0
    global LSB
    LSB = 0.0
    global Rg
    Rg = 0.0
    global Ro
    Ro = 0.0
    
    massSet = [0.0,
               3.72,   7.46,   11.15,  14.87,  18.58,
               22.28,   26.03,  29.74,  33.46,  37.18,
               40.91,   44.63,  48.40,  52.12,  55.85,
               59.58,   63.29,  67.02,  70.73,  74.44,
               78.15,   81.85,  85.55,  89.23,  92.93,
               96.67,   100.41, 104.14, 107.86, 111.58,
               115.25,  118.96, 122.66, 126.36, 130.08,
               133.82,  137.56, 141.31, 145.00, 148.73,
               152.45,  156.18, 159.88, 163.61, 167.34]
    forceSet = [None]*len(massSet)
    momentSet = [None]*len(massSet)
    stressSet = [None]*len(massSet)
    strainSet = [None]*len(massSet)
    VwsbSet = [None]*len(massSet)
    VainSet = [None]*len(massSet)
    DadcSet = [None]*len(massSet)
    DadcRatSet = [None]*len(massSet)
    for i in range(0,len(massSet)):
        forceSet[i] = massSet[i]*g/1000.0
        momentSet[i] = forceSet[i]*(xf-xs)
        stressSet[i] = momentSet[i]*c/Ix
        strainSet[i] = stressSet[i]/E
        VwsbSet[i] = (1.0/2.0)*strainSet[i]*S*Vs
        VainSet[i] = VwsbSet[i]*Gia

    #---------------Page Variables---------------------------------------------------------------------------

        #generalString = tk.StringVar(master=window)
    lines = tk.StringVar(master=window)
    prevLine = tk.StringVar(master=window)
    indText = tk.StringVar(master=window, value=port+': No Connection')
    datID = tk.StringVar(master=window)
    varID = tk.StringVar(master=window, value='Input Data: ( )')
    plotID = tk.StringVar(master=window, value='y = f(x)')
    head = tk.StringVar(master=window)
    input1 = tk.StringVar(master=window)
    input2 = tk.StringVar(master=window)
    input3 = tk.StringVar(master=window)
    input4 = tk.StringVar(master=window)
    input5 = tk.StringVar(master=window)
    par1 = tk.StringVar(master=window)
    par2 = tk.StringVar(master=window)
    horizDatID = tk.StringVar(master=window, value='X Config')
    vertDatID = tk.StringVar(master=window, value='Y Config')
    xScalarS = tk.StringVar(master=window, value='1.0')
    yScalarS = tk.StringVar(master=window, value='1.0')
    yOffsetS = tk.StringVar(master=window, value='0.0')
    lblRg = tk.StringVar(master=window)
    lblRo = tk.StringVar(master=window)
    lblVfs = tk.StringVar(master=window)
    lblVspan = tk.StringVar(master=window)
    lblLSB = tk.StringVar(master=window)
    
        #generalBoolean = tk.BooleanVar(master=window, value=False)
    status = tk.BooleanVar(master=window, value=True)
    datWin = tk.BooleanVar(master=window, value=True)
    datAct = tk.BooleanVar(master=window, value=False)
    datRead = tk.BooleanVar(master=window, value=False)
    datWrite = tk.BooleanVar(master=window, value=False)
    datReady = tk.BooleanVar(master=window, value=False)
    varWin = tk.BooleanVar(master=window, value=True)
    plotWin = tk.BooleanVar(master=window, value=True)
    plotAct = tk.BooleanVar(master=window, value=False)
    rawAct = tk.BooleanVar(master=window, value=True)
    aveAct = tk.BooleanVar(master=window, value=True)
    trendAct = tk.BooleanVar(master=window, value=True)
    legAct = tk.BooleanVar(master=window, value=True)
    realAct = tk.BooleanVar(master=window, value=False)
    invPlot = tk.BooleanVar(master=window, value=False)
    xTypeA = tk.BooleanVar(master=window, value=True)
    yTypeA = tk.BooleanVar(master=window, value=False)

        #generalInt = tk.IntVar(master=window, value=0)
    auto = tk.IntVar(master=window, value=1)
    ts = tk.IntVar(master=window, value=0)  #timestamp
    datRead = tk.IntVar(master=window, value=0)
    rst = tk.IntVar(master=window, value=0)
    page = tk.IntVar(master=window, value=1)
    datSel = tk.IntVar(master=window, value=6)
    index = tk.IntVar(master=window, value=0)
    nPars = tk.IntVar(master=window, value=0)
    nSamp = tk.IntVar(master=window, value=0)
    module = tk.IntVar(master=window, value=0)
    calPlotType = tk.IntVar(master=window, value=0)
    xType = tk.IntVar(master=window, value=0)
    yType = tk.IntVar(master=window, value=0)
    yTypeB = tk.IntVar(master=window, value=0)

        #generalDouble = tk.DoubleVar(master=window, value=0.0)
    y0 = tk.DoubleVar(master=window, value=0.0)
    y1 = tk.DoubleVar(master=window, value=0.0)

    #---------------Page Commands---------------------------------------------------------------------------
    def setup():
        print('Setup')
        calcGain()
        calcOffset()
        calcVolts()
        try:
            plotter()
            window.update_idletasks()
            window.update()
            screenPrint("\n\n===== SERIAL PORT CONNECTED =====\n\n")
            indicator.configure(image = onInd)
            indicator.image = onInd
            plotterIndicator.configure(image = onInd)
            plotterIndicator.image = onInd
            indText.set(port+': Active')
            status.set(True)
            showData()
        except:
            screenPrint("\n\n===== SERIAL PORT NOT CONNECTED =====\n\n")
            indicator.configure(image = offInd)
            indicator.image = offInd
            plotterIndicator.configure(image = offInd)
            plotterIndicator.image = offInd
            indText.set(port+': No Connection')
            status.set(False)
            
    def main():
        while True:
            if status.get():
                if serial_port.inWaiting()==0:
                    time.sleep(.01)
                else:
                    prevLine.set(lines.get())
                    lines.set(serial_port.readline().decode())
                    screenPrint(lines.get())
            try:
                #if status.get():
                #    if not serial_port.inWaiting()==0:
                #        lines.set(serial_port.readline().decode())
                #        screenPrint(lines.get())
                if(screen.yview()[1]-screen.yview()[0])<1.0:
                    textScroll.config(command = screen.yview)
                if(dataScreen.yview()[1]-dataScreen.yview()[0])<1.0:
                    dataWindowScroll.config(command = dataScreen.yview)
                        
                window.update_idletasks()
                window.update()
            except:
                break

    def reset():
        print('Reset')
        screenPrint("\n\n===== RESET SERIAL PORT: "+port+" =====\n\n")
        serial_port.close()
        status.set(False)
        serial_port.open()
        status.set(True)
        connectButton.configure(text='Disconnect', fg='red')
        plotterConnectButton.configure(text='Disconnect', fg='red')
        screenPrint("\n\n===== SERIAL PORT CONNECTED =====\n\n")
        print(f"The Port name is {serial_port.name}")

    def clear():
        screen.config(state=tk.NORMAL)
        screen.delete(1.0,tk.END)
        screen.config(state=tk.DISABLED)

    def portConnect():
        print('Port Connect')
        status.set(not status.get())
        print(status.get())
        if status.get():
            serial_port.open()
            connectButton.configure(text='Disconnect', fg='red')
            plotterConnectButton.configure(text='Disconnect', fg='red')
            screenPrint("\n\n===== SERIAL PORT CONNECTED =====\n\n")
            indicator.configure(image = onInd)
            indicator.image = onInd
            plotterIndicator.configure(image = onInd)
            plotterIndicator.image = onInd
            indText.set(port+': Active')
            status.set(True)
        else:
            serial_port.close()
            connectButton.configure(text='Connect', fg='green')
            plotterConnectButton.configure(text='Connect', fg='green')
            screenPrint("\n\n===== SERIAL PORT DISCONNECTED =====\n\n")
            indicator.configure(image = offInd)
            indicator.image = offInd
            indText.set(port+': No Connection')
            plotterIndicator.configure(image = offInd)
            plotterIndicator.image = offInd
            status.set(False)

    def screenPrint(out):
        #print('Screen Print')
        screen.config(state=tk.NORMAL)
        screen.insert(tk.INSERT, out)
        if auto.get():
            screen.see("end")
        screen.config(state=tk.DISABLED)
        if datSel.get()==6:
            dataScreen.insert(tk.INSERT, out)
            dataScreen.see("end")
        if "#START" in out:
            datAct.set(True)
            datRead.set(False)
            index.set(0)
            serFile.seek(0)
            serFile.truncate(0)
            #serCSV.truncate(0)
            datAllButton.pack_forget()
            if not module.get()==2:
                plotWin.set(False)
                plotWindowBox.pack_forget()
                showPlot()
            if "Element 0" in prevLine.get():
                print("Calibration Curve")
                module.set(2)
                setPlot()
        elif "#END" in out:
            datAct.set(False)
            print("Data Collected")
            logCSV()
            if plotAct.get() and module.get()==2:
                plot()
        else:
            if datAct.get():
                serFile.write(out.rstrip()+'\n')

    def monitor():
        print('Monitor')
        plotterFrame.pack_forget()
        monitorFrame.pack(side=tk.BOTTOM, fill=tk.BOTH)

    def plotter():
        print('Plotter')
        monitorFrame.pack_forget()
        plotterFrame.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)
        setPlot()

    def calcGain():
        global Rg
        Rg = 0.0
        d32 = [None]*32
        bits = 'Gain:\t'
        for i in range(0,32):
            d32[i] = (RgD>>i)&1
        for i in range(0,32):
            bits = bits + str(d32[31-i])
        #print(bits)
        for i in range(0,30):
            Rg = Rg + d32[i]*pow(2,(i-24))
        lblRg.set('Rg\t= '+"{:.6f}".format(Rg))
        #print(Rg)
        print(lblRg.get())

    def calcOffset():
        global Ro
        Ro = 0.0
        d32 = [None]*32
        bits = 'Offset:\t'
        for i in range(0,32):
            d32[i] = (RoD>>i)&1
        for i in range(0,32):
            bits = bits + str(d32[31-i])
        #print(bits)
        for i in range(8,31):
            Ro = Ro + d32[i]*pow(2,(i-32))
        Ro = Ro - d32[31]*pow(2,-1)
        lblRo.set('Ro\t= '+"{:.6f}".format(Ro))
        #print(Ro)
        print(lblRo.get())

    def calcVolts():
        global Vfs
        global Vspan
        global LSB
        Vfs = Vref/(Gadc*A*Rg)
        Vspan = Vfs*Kbi
        LSB = pow(10,9)*Vspan/pow(2,N)
        lblVfs.set('Vfs\t= '+"{:.6f}".format(Vfs)+' V')
        lblVspan.set('Vspan\t= '+"{:.6f}".format(Vspan)+' V')
        lblLSB.set('LSB\t= '+"{:.6f}".format(LSB)+' nV')
        #print(Vfs)
        #print(Vspan)
        #print(LSB)
        print(lblVfs.get())
        print(lblVspan.get())
        print(lblLSB.get())
        for i in range(0,len(massSet)):
            DadcSet[i] = math.ceil(VainSet[i]/LSB)
            DadcRatSet[i] = DadcSet[i]/pow(2.0,23)

    def plot():
        print('Set Plot')
            
        if module.get()==0:
            print('y = f(x)')
            if par1.get()==input1.get():
                y = v1
            elif par1.get()==input2.get():
                y = v2
            elif par1.get()==input3.get():
                y = v3
            elif par1.get()==input4.get():
                y = v4
            elif par1.get()==input5.get():
                y = v5

            if par2.get()==input1.get():
                x = v1
            elif par2.get()==input2.get():
                x = v2
            elif par2.get()==input3.get():
                x = v3
            elif par2.get()==input4.get():
                x = v4
            elif par2.get()==input5.get():
                x = v5
            #fig = Figure(dpi=96)
            #plot1 = fig.add_subplot(111)
            plot1.plot(x,y)
            #canvas = FigureCanvasTkAgg(fig, master=plotterCanvasFrame)
            canvas.draw()
            plotWin.set(True)
            plotWindowBox.pack(side=tk.TOP, pady=10, padx=20, anchor='nw')
            showPlot()
            canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

        elif module.get()==1:
            print('Live Plot')
            print('Not yet available')

        elif module.get()==2:
            print('Calibration Curve')
            if invPlot.get():
                horizDatID.set('Y Config')
                vertDatID.set('X Config')
            else:
                horizDatID.set('X Config')
                vertDatID.set('Y Config')
            if xType.get()==0 and yType.get()==0 and (not yTypeA.get()):
                df = pd.read_csv('SerialData.csv')
            else:
                calPlotAdjust()
                df = pd.read_csv('StrainCalData.csv')
            serFile.seek(0)
            serCSV.seek(0)
            data = serFile.readlines()
            fig.clf()
            plot1 = fig.add_subplot(111)
            x = [None]*(nPars.get()+1)
            y = [None]*(nPars.get()+1)
            realX = [None]*(nPars.get()+1)
            realY = [None]*(nPars.get()+1)
            hands = ()
            labs = ()
            nSamp.set(len(df['0']))
            print(nSamp.get())
            for i in range(0, nPars.get()+1):
                if invPlot.get():
                    if xType.get()==1 and xTypeA.get():
                        y[i] = massSet[i]#*9.81/1000
                    else:
                        y[i] = i
                    x[i] = st.mean(df[str(i)])
                    if rawAct.get():
                        p0, = plot1.plot(df[str(i)],y[i]*np.ones(len(df[str(i)])),linestyle='',marker='.',
                                         markeredgecolor='blue',markerfacecolor='blue')
                else:
                    if xType.get()==1 and xTypeA.get():
                        x[i] = massSet[i]#*9.81/1000
                    else:
                        x[i] = i
                    y[i] = st.mean(df[str(i)])
                    if rawAct.get():
                        p0, = plot1.plot(x[i]*np.ones(len(df[str(i)])),df[str(i)],linestyle='',marker='.',
                                         markeredgecolor='blue',markerfacecolor='blue')
            if rawAct.get():
                hands = hands + (p0,)
                labs = labs + ('Raw Data',)
            canvas.draw()
            if aveAct.get():
                p1, = plot1.plot(x,y,marker='o',markeredgecolor='black',markerfacecolor='white',linestyle='',color='black')
                canvas.draw()
                hands = hands + (p1,)
                labs = labs + ('Averages',)
            if nPars.get()>0:
                z = np.polyfit(x,y,1)
                p = np.poly1d(z)
                if z[1]>0:
                    eq = "y = %.2fx + %.2f"%(z[0],z[1])
                else:
                    eq = "y = %.2fx - %.2f"%(z[0],abs(z[1]))
                print(eq)
                if trendAct.get():
                    p2, = plot1.plot(x,p(x),linestyle='--',color='black')
                    canvas.draw()
                    hands = hands + (p2,)
                    labs = labs + (eq,)
                if legAct.get():
                    fig.legend(hands, labs, 'upper right')
                    canvas.draw()
            if invPlot.get():
                plot1.set_ylabel(par1.get(), fontsize=16)
                plot1.set_xlabel(par2.get(), fontsize=16)
            else:
                plot1.set_xlabel(par1.get(), fontsize=16)
                plot1.set_ylabel(par2.get(), fontsize=16)
            #if
            canvas.draw()
            plotWin.set(True)
            plotWindowBox.pack(side=tk.TOP, pady=10, padx=20, anchor='nw')
            showPlot()
            
    def setPlot():
        if module.get()==0:
            plotID.set('y = f(x)')
            plotterSelect1.pack(side=tk.LEFT)
            variableSelectSpacer.pack(side=tk.LEFT)
            plotterSelect2.pack(side=tk.LEFT)
        elif module.get()==1:
            plotID.set('Live Plot')
            plotterSelect1.pack(side=tk.LEFT)
            variableSelectSpacer.pack(side=tk.LEFT)
            plotterSelect2.pack(side=tk.LEFT)
        elif module.get()==2:
            plotID.set('Calibration Curve')
            plotterSelect1.pack_forget()
            variableSelectSpacer.pack_forget()
            plotterSelect2.pack_forget()
        elif module.get()==3:
            plotID.set('FFT')
            plotterSelect1.pack(side=tk.LEFT)
            variableSelectSpacer.pack(side=tk.LEFT)
            plotterSelect2.pack(side=tk.LEFT)

    def xConfig():
        if xType.get()==1:
            xMassBox.pack(anchor='nw',padx=20)
            if xTypeA.get():
                plotterCalXLabelFrame.pack_forget()
                plotterCalXScalarFrame.pack_forget()
                par1.set('Mass [g]')
            else:
                par1.set('Parameter [units]')
                plotterCalXLabelFrame.pack(anchor='e',padx=10)
                plotterCalXScalarFrame.pack(anchor='e',padx=10)
        else:
            par1.set('Element [n]')
            xMassBox.pack_forget()
            plotterCalXLabelFrame.pack_forget()
            plotterCalXScalarFrame.pack_forget()
        plot()

    def yConfig():
        if yType.get()==1:
            yRatioBox.pack_forget()
            yCustomButton.pack(anchor='w', padx=20)
            yADCButton.pack(anchor='w', padx=20)
            if yTypeB.get()==0:
                par2.set('Parameter [units]')
                yScalarS.set('1.0')
                plotterCalYRgLabel.pack_forget()
                plotterCalYRoLabel.pack_forget()
                plotterCalYVfsLabel.pack_forget()
                plotterCalYVspanLabel.pack_forget()
                plotterCalYLSBLabel.pack_forget()
                plotterCalYLabelFrame.pack(anchor='e',padx=10)
                plotterCalYScalarFrame.pack(anchor='e',padx=10)
                plotterCalYOffsetFrame.pack(anchor='e',padx=10)
            elif yTypeB.get()==1:
                plotterCalYLabelFrame.pack_forget()
                plotterCalYScalarFrame.pack_forget()
                plotterCalYOffsetFrame.pack_forget()
                plotterCalYRgLabel.pack(anchor='w',padx=10)
                plotterCalYRoLabel.pack(anchor='w',padx=10)
                plotterCalYVfsLabel.pack(anchor='w',padx=10)
                plotterCalYVspanLabel.pack(anchor='w',padx=10)
                plotterCalYLSBLabel.pack(anchor='w',padx=10)
                par2.set('Voltage [mV]')
                yScalarS.set(str(LSB/pow(10,6)))
        else:
            yCustomButton.pack_forget()
            yADCButton.pack_forget()
            yRatioBox.pack(anchor='w',padx=15)
            plotterCalYLabelFrame.pack_forget()
            plotterCalYScalarFrame.pack_forget()
            plotterCalYOffsetFrame.pack_forget()
            if yTypeA.get():
                text1 = "ADC Output Ratio ["
                text2 = r'$\frac{{D}}{{2^{{{}}} }}$'.format(23)
                text3 = ("]")
                par2.set(text1+text2+text3)
                yScalarS.set('1/8388608')
            else:
                par2.set(("ADC Output $[-2^{{{}}} \leq D \leq 2^{{{}}}]$").format(23,23))
                yScalarS.set('1.0')
        plot()

    def xConfigM(event):
        xEntry.select_range(0,len(par1.get()))

    def yConfigM(event):
        yEntry.select_range(0,len(par2.get()))

    def configK(event):
        plot()

    def calPlotConfig():
        print(calPlotType.get())

    def calPlotAdjust():
        xG = float(xScalarS.get())
        if '/' in yScalarS.get():
            num = float(yScalarS.get().partition('/')[0])
            den = float(yScalarS.get().partition('/')[2])
            yG = num/den
            print(num,' / ',den,' = ',yG)
        else:
            yG = float(yScalarS.get())
        yO = float(yOffsetS.get())
        serCSV.seek(0)
        strainCSV.seek(0)
        df = pd.read_csv('SerialData.csv')
        try:
            dfCal = pd.read_csv('StrainCalData.csv')
        except:
            dfCal = pd.DataFrame()
        serCSV.seek(0)
        strainCSV.seek(0)
        for i in range(0, nPars.get()+1):
            dfCal[str(i)] = (df[str(i)]+yO)*yG

        dfCal.to_csv('StrainCalData.csv',index=False,header=head.get())

    def calPlotAdjustK(event):
        calPlotAdjust()

    def addPlotButtonK(event):
        if module.get()==0:
            if (not par1.get()=='') and (not par2.get()=='') and datReady.get():
                plotButton.pack(side=tk.LEFT)#, padx=20)
            else:
                plotButton.pack_forget()
        elif module.get()==2:
            plotterVariableNameFrame.pack_forget()
            plotButton.pack(side=tk.LEFT)#, padx=20)
            plotterCalPropFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
            plotterCalXYFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
            input1.set('Mass [g]')
            addTab()
            v1[:] = massSet

    def addPlotButton():
        if module.get()==0:
            if (not par1.get()=='') and (not par2.get()=='') and datReady.get():
                plotButton.pack(side=tk.LEFT)#, padx=20)
            else:
                plotButton.pack_forget()
        elif module.get()==2:
            plotterVariableNameFrame.pack_forget()
            plotButton.pack(side=tk.LEFT)#, padx=20)
            plotterCalPropFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
            plotterCalXYFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
            input1.set('Mass [g]')
            addTab()
            v1[:] = massSet

    def showData():
        if datWin.get():
            plotterDataWindowFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        else:
            plotterDataWindowFrame.pack_forget()

    def showPlot():
        if plotWin.get():
            plotterCanvasFrame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
            if datWin.get():
                plotterDataWindowFrame.pack_forget()
                plotterDataWindowFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        else:
            plotterCanvasFrame.pack_forget()

    def showVar():
        if varWin.get():
            plotterVariableFrame.pack(side=tk.BOTTOM, fill=tk.BOTH)
        else:
            plotterVariableFrame.pack_forget()

    def datWinX():
        plotterDataWindowFrame.pack_forget()
        datWin.set(False)

    def addTab():
        dat1Button.pack_forget()
        dat2Button.pack_forget()
        dat3Button.pack_forget()
        dat4Button.pack_forget()
        dat5Button.pack_forget()
        if not input1.get()=='':
            dat1Button.pack(side=tk.LEFT)
        if not input2.get()=='':
            dat2Button.pack(side=tk.LEFT)
        if not input3.get()=='':
            dat3Button.pack(side=tk.LEFT)
        if not input4.get()=='':
            dat4Button.pack(side=tk.LEFT)
        if not input5.get()=='':
            dat5Button.pack(side=tk.LEFT)

    def logCSV():
        print('LogCSV')
        serFile.seek(0)
        serCSV.seek(0)
        data = [line.rstrip('\n') for line in serFile]#serFile.readlines()
        i = 0
        if module.get()==2:
            print("Logging Element")
            try:
                df = pd.read_csv('SerialData.csv')
            except:
                df = pd.DataFrame()
            dat = data[1:]
            ele = data[0].rstrip()
            if not ele in df.columns:
                nPars.set(int(ele))
                head.set('0')
                if int(ele) > 0:
                    for i in range(1,int(ele)+1):
                        head.set(head.get() + ',' + str(i))
                        print(head.get())
                    head.set(head.get().split(','))
            print(head.get())
            df[ele] = dat
            df.to_csv('SerialData.csv',index=False,header=head.get())
            addPlotButton()
            xConfig()
            yConfig()
        else:
            datID.set(data[0].rstrip())
            #print(datID.get())
            varID.set('Input Data: ('+datID.get()+')')
            nPars.set(data[0].count(',')+1)
            #print(nPars.get())
            for line in data:
                serCSVWriter.writerow(line.rstrip().split(','))
            serFile.seek(0)
            serCSV.seek(0)
            df = pd.read_csv('SerialData.csv')
            for col in df:
                if input1.get()=='':
                    input1.set(col)
                elif input2.get()=='':
                    input2.set(col)
                elif input3.get()=='':
                    input3.set(col)
                elif input4.get()=='':
                    input4.set(col)
                elif input5.get()=='':
                    input5.set(col)
            
            if not input1.get()=='':
                v1[:] = df[input1.get()]
            if not input2.get()=='':
                v2[:] = df[input2.get()]
            if not input3.get()=='':
                v3[:] = df[input3.get()]
            if not input4.get()=='':
                v4[:] = df[input4.get()]
            if not input5.get()=='':
                v5[:] = df[input5.get()]
            serFile.seek(0)
            datReady.set(True)
            datAllButton.pack(side=tk.LEFT)
            addTab()
            addPlotButton()

    def datPrint():
        #print('Screen Print')
        dataScreen.delete(1.0, tk.END)

        if module.get()==0:
            if datSel.get()==6:
                print("Plotter Serial Monitor")
                dataScreen.insert(tk.INSERT, screen.get(1.0, tk.END))
                dataScreen.see("end")
            elif datSel.get()==7:
                print("All Data")
                df = pd.read_csv('SerialData.csv')
                dataScreen.insert(tk.INSERT, (datID.get()+'\n'))
                for i in df.index:
                    out = str(v1[i])+', '+str(v2[i])+', '+str(v3[i])+'\n'
                    dataScreen.insert(tk.INSERT, out)
                serFile.seek(0)
                serCSV.seek(0)
            elif datSel.get()==1:
                for line in v1:
                    dataScreen.insert(tk.INSERT, line)
                    dataScreen.insert(tk.INSERT, '\n')
            elif datSel.get()==2:
                for line in v2:
                    dataScreen.insert(tk.INSERT, line)
                    dataScreen.insert(tk.INSERT, '\n')
            elif datSel.get()==3:
                for line in v3:
                    dataScreen.insert(tk.INSERT, line)
                    dataScreen.insert(tk.INSERT, '\n')
            elif datSel.get()==4:
                for line in v4:
                    dataScreen.insert(tk.INSERT, line)
                    dataScreen.insert(tk.INSERT, '\n')
            elif datSel.get()==5:
                for line in v5:
                    dataScreen.insert(tk.INSERT, line)
                    dataScreen.insert(tk.INSERT, '\n')
        elif module.get()==2:
            if datSel.get()==6:
                print("Plotter Serial Monitor")
                dataScreen.insert(tk.INSERT, screen.get(1.0, tk.END))
                dataScreen.see("end")
            elif datSel.get()==7:
                print("All Data")
                df = pd.read_csv('SerialData.csv')
                dataScreen.insert(tk.INSERT, (datID.get()+'\n'))
                for i in df.index:
                    out = str(v1[i])+', '+str(v2[i])+', '+str(v3[i])+'\n'
                    dataScreen.insert(tk.INSERT, out)
                serFile.seek(0)
                serCSV.seek(0)
            elif datSel.get()==1:
                for i in range(0,len(massSet)):
                    dataScreen.insert(tk.INSERT, (str(v1[i])+'\n'))
                    #dataScreen.insert(tk.INSERT, ('N'+str(i)+'\t=  '+str(v1[i])+' g\n'))

    #----------------Page Widgets---------------------------------------------------------------------------

        #generalFrame = tk.Frame(master=window, bg=color, relief=border, borderwidth=bw)
    body                            = tk.Frame(master=window, relief=border, borderwidth=bw)#, bg=color)
    monitorFrame                    = tk.Frame(master=body, relief=border, borderwidth=bw)
    uiFrame                         = tk.Frame(master=monitorFrame, relief=border, borderwidth=bw)
    headerFrame                     = tk.Frame(master=body,relief=border, borderwidth=bw)
    boxFrame                        = tk.Frame(master=uiFrame)
    uiButtonFrame                   = tk.Frame(master=uiFrame)
    uiIndicatorFrame                = tk.Frame(master=uiFrame)
    uiIndicatorLabelFrame           = tk.Frame(master=uiIndicatorFrame)
    headerButtonFrame               = tk.Frame(master=headerFrame, relief=border, borderwidth=bw)
    printFrame                      = tk.Frame(master=monitorFrame, relief=border, borderwidth=bw)

    plotterFrame                    = tk.Frame(master=body, relief=border, borderwidth=bw)
    plotterUIFrame                  = tk.Frame(master=plotterFrame, relief=border, borderwidth=bw)
    plotterUIButtonFrame            = tk.Frame(master=plotterUIFrame)
    plotterUIIndicatorFrame         = tk.Frame(master=plotterUIFrame)
    plotterUIIndicatorLabelFrame    = tk.Frame(master=plotterUIIndicatorFrame)
    plotterDisplayFrame             = tk.Frame(master=plotterFrame)
    plotterCanvasFrame              = tk.Frame(master=plotterDisplayFrame, relief=border, borderwidth=bw)
    plotterVariableFrame            = tk.Frame(master=plotterFrame, relief=border, borderwidth=bw)
    plotterVariableNameFrame        = tk.Frame(master=plotterVariableFrame, relief=border, borderwidth=bw)#
    plotterVariableEntryFrame       = tk.Frame(master=plotterVariableNameFrame, relief=border, borderwidth=bw)#
    plotterVariableSelectFrame      = tk.Frame(master=plotterVariableNameFrame, relief=border, borderwidth=bw)#
    plotterVariableBoxFrame         = tk.Frame(master=plotterVariableFrame, relief=border, borderwidth=bw)
    plotterPlotSelectFrame          = tk.Frame(master=plotterVariableFrame, relief=border, borderwidth=bw)#
    plotterPlotSelectTextFrame      = tk.Frame(master=plotterPlotSelectFrame, relief=border, borderwidth=bw)#
    plotterPlotSelectUIFrame        = tk.Frame(master=plotterPlotSelectFrame, relief=border, borderwidth=bw)#
    plotterDataWindowFrame          = tk.Frame(master=plotterDisplayFrame, relief=border, borderwidth=bw)

    plotterCalPropFrame             = tk.Frame(master=plotterVariableFrame, relief=border, borderwidth=bw)#
    plotterCalPropBoxFrame          = tk.Frame(master=plotterCalPropFrame, relief=border, borderwidth=bw)#
    plotterCalPropRadioFrame        = tk.Frame(master=plotterCalPropFrame, relief=border, borderwidth=bw)#
    plotterCalXYFrame               = tk.Frame(master=plotterVariableFrame)#, relief=border, borderwidth=bw)#
    plotterCalXFrame                = tk.Frame(master=plotterCalXYFrame, relief=border, borderwidth=bw)#
    plotterCalXTitleFrame           = tk.Frame(master=plotterCalXFrame)#, relief=border, borderwidth=bw)#
    plotterCalXBodyFrame            = tk.Frame(master=plotterCalXFrame)#, relief=border, borderwidth=bw)#
    plotterCalXTopFrame             = tk.Frame(master=plotterCalXBodyFrame)#, relief=border, borderwidth=bw)#
    plotterCalXBotFrame             = tk.Frame(master=plotterCalXBodyFrame)#, relief=border, borderwidth=bw)#
    plotterCalXBoxFrame             = tk.Frame(master=plotterCalXBotFrame)#, relief=border, borderwidth=bw)#
    plotterCalXEntryFrame           = tk.Frame(master=plotterCalXBotFrame)#, relief=border, borderwidth=bw)#
    plotterCalXLabelFrame           = tk.Frame(master=plotterCalXEntryFrame)#, relief=border, borderwidth=bw)#
    plotterCalXScalarFrame          = tk.Frame(master=plotterCalXEntryFrame)#, relief=border, borderwidth=bw)#

    plotterCalYFrame                = tk.Frame(master=plotterCalXYFrame, relief=border, borderwidth=bw)#
    plotterCalYTitleFrame           = tk.Frame(master=plotterCalYFrame)#, relief=border, borderwidth=bw)#
    plotterCalYBodyFrame            = tk.Frame(master=plotterCalYFrame)#, relief=border, borderwidth=bw)#
    plotterCalYTopFrame             = tk.Frame(master=plotterCalYBodyFrame)#, relief=border, borderwidth=bw)#
    plotterCalYBotFrame             = tk.Frame(master=plotterCalYBodyFrame)#, relief=border, borderwidth=bw)#
    plotterCalYEntryFrame           = tk.Frame(master=plotterCalYBotFrame)#, relief=border, borderwidth=bw)#
    plotterCalYButton2Frame         = tk.Frame(master=plotterCalYBotFrame)#, relief=border, borderwidth=bw)#
    plotterCalYLabelFrame           = tk.Frame(master=plotterCalYEntryFrame)#, relief=border, borderwidth=bw)#
    plotterCalYScalarFrame          = tk.Frame(master=plotterCalYEntryFrame)#, relief=border, borderwidth=bw)#
    plotterCalYOffsetFrame          = tk.Frame(master=plotterCalYEntryFrame)#, relief=border, borderwidth=bw)#

    dataWindowHeaderFrame           = tk.Frame(master=plotterDataWindowFrame, relief=border, borderwidth=bw)
    dataWindowPrintFrame            = tk.Frame(master=plotterDataWindowFrame, relief=border, borderwidth=bw)
    dataWindowHeaderSelectFrame     = tk.Frame(master=dataWindowHeaderFrame, relief=border, borderwidth=bw)#
    dataWindowXButtonFrame          = tk.Frame(master=dataWindowHeaderFrame)
    
        #generalLabel = tk.Label(master=generalLabelFrame, text="General", bg=color, font=MEDFONT, relief=border)
    indicator               = tk.Label(master=uiIndicatorFrame, image=offInd)
    indicatorLabel          = tk.Label(master=uiIndicatorLabelFrame, textvariable=indText, font=BUTTONFONT, width=18, anchor='w')

    plotterIndicator        = tk.Label(master=plotterUIIndicatorFrame, image=offInd)
    plotterIndicatorLabel   = tk.Label(master=plotterUIIndicatorLabelFrame, textvariable=indText, font=BUTTONFONT, width=18, anchor='w')

    variableIDLabel         = tk.Label(master=plotterVariableEntryFrame, textvariable=varID, font=LARGEFONT)
    
    moduleLabel           = tk.Label(master=plotterPlotSelectTextFrame, textvariable=plotID, font=LARGEFONT)

    variableSelectLabel     = tk.Label(master=plotterVariableSelectFrame, text='Plot Data:', font=LARGEFONT)
    variableSelectSpacer    = tk.Label(master=plotterVariableSelectFrame, text='vs', font=LARGEFONT)

    plotterCalXLabel        = tk.Label(master=plotterCalXTitleFrame, textvariable=horizDatID, font=LARGEFONT)
    plotterCalXLabelLabel   = tk.Label(master=plotterCalXLabelFrame, text='Label: ', font=MEDFONT)
    plotterCalXScalarLabel  = tk.Label(master=plotterCalXScalarFrame, text='Scalar: ', font=MEDFONT)
    plotterCalYLabel        = tk.Label(master=plotterCalYTitleFrame, textvariable=vertDatID, font=LARGEFONT)
    plotterCalYLabelLabel   = tk.Label(master=plotterCalYLabelFrame, text='Label: ', font=MEDFONT)
    plotterCalYScalarLabel  = tk.Label(master=plotterCalYScalarFrame, text='Scalar: ', font=MEDFONT)
    plotterCalYOffsetLabel  = tk.Label(master=plotterCalYOffsetFrame, text='Offset: ', font=MEDFONT)
    plotterCalYRgLabel      = tk.Label(master=plotterCalYEntryFrame, textvariable=lblRg, font=BUTTONFONT)
    plotterCalYRoLabel      = tk.Label(master=plotterCalYEntryFrame, textvariable=lblRo, font=BUTTONFONT)
    plotterCalYVfsLabel     = tk.Label(master=plotterCalYEntryFrame, textvariable=lblVfs, font=BUTTONFONT)
    plotterCalYVspanLabel   = tk.Label(master=plotterCalYEntryFrame, textvariable=lblVspan, font=BUTTONFONT)
    plotterCalYLSBLabel     = tk.Label(master=plotterCalYEntryFrame, textvariable=lblLSB, font=BUTTONFONT)
    
        #generalCanvas = tk.Canvas(master=generalCanvasFrame)
    plotterCanvas           = tk.Canvas(master=plotterCanvasFrame, bg=color)
    #plotterCanvas           = FigureCanvasTkAgg(master=plotterCanvasFrame)
    fig = Figure(dpi=96)
    plot1 = fig.add_subplot(111)
    canvas = FigureCanvasTkAgg(fig, master=plotterCanvasFrame)
    canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

        #generalScroll = tk.Scrollbar(master=generalScrollFrame, bg=color, variable=sc1)
    textScroll              = tk.Scrollbar(master=printFrame, orient=tk.VERTICAL)
    dataWindowScroll        = tk.Scrollbar(master=dataWindowPrintFrame, orient=tk.VERTICAL)

        #generalEntry = tk.Entry(master=generalEntryFrame, textvariable=strvar1, command=readtext)
    plotterSelect1          = tk.Entry(master=plotterVariableSelectFrame, textvariable=par1, font=MEDFONT, width=5, bd=4)
    plotterSelect2          = tk.Entry(master=plotterVariableSelectFrame, textvariable=par2, font=MEDFONT, width=5, bd=4)
    plotterSelect1.bind('<Any-KeyRelease>',addPlotButtonK)
    plotterSelect2.bind('<Any-KeyRelease>',addPlotButtonK)

    xLabelEntry = tk.Entry(master=plotterCalXLabelFrame, textvariable=par1, font=BUTTONFONT, width=15, bd=4)
    xScalarEntry = tk.Entry(master=plotterCalXScalarFrame, textvariable=xScalarS, font=BUTTONFONT, width=15, bd=4)
    yLabelEntry = tk.Entry(master=plotterCalYLabelFrame, textvariable=par2, font=BUTTONFONT, width=15, bd=4)
    yScalarEntry = tk.Entry(master=plotterCalYScalarFrame, textvariable=yScalarS, font=BUTTONFONT, width=15, bd=4)
    yOffsetEntry = tk.Entry(master=plotterCalYOffsetFrame, textvariable=yOffsetS, font=BUTTONFONT, width=15, bd=4)
    xLabelEntry.bind('<Any-KeyRelease>',configK)
    yLabelEntry.bind('<Any-KeyRelease>',configK)
    xLabelEntry.bind('<ButtonRelease-1>',xConfigM)
    yLabelEntry.bind('<ButtonRelease-1>',yConfigM)
    xScalarEntry.bind('<Any-KeyRelease>',calPlotAdjustK)
    yScalarEntry.bind('<Any-KeyRelease>',calPlotAdjustK)
    yOffsetEntry.bind('<Any-KeyRelease>',calPlotAdjustK)

        #generalText = tk.Text(master=generalTextFrame)
    screen = tk.Text(master=printFrame, font=BUTTONFONT, state=tk.NORMAL, height=64,
                     yscrollcommand=textScroll.set)
    dataScreen = tk.Text(master=dataWindowPrintFrame, font=BUTTONFONT, state=tk.NORMAL,
                         yscrollcommand=dataWindowScroll.set)

        #generalButton = tk.Button(master=generalButtonFrame, text="General", command=buttonPush, font=LARGEFONT)
    clearButton = tk.Button(master=uiButtonFrame, text="Clear Output", font=BUTTONFONT, height=buh, width=buw,
                            image=pixel, pady=0, padx=0, compound="center", command=clear)
    resetButton = tk.Button(master=uiButtonFrame, text="Reset Port", font=BUTTONFONT, height=buh, width=buw,
                            image=pixel, pady=0, padx=0, compound="center", command=reset)
    connectButton = tk.Button(master=uiButtonFrame, text="Disconnect", font=BUTTONFONT, height=buh, width=buw,
                            image=pixel, pady=0, padx=0, compound="center", command=portConnect, fg='red')

    plotterClearButton = tk.Button(master=plotterUIButtonFrame, text="Clear Output", font=BUTTONFONT, height=buh, width=buw,
                            image=pixel, pady=0, padx=0, compound="center", command=clear)
    plotterResetButton = tk.Button(master=plotterUIButtonFrame, text="Reset Port", font=BUTTONFONT, height=buh, width=buw,
                            image=pixel, pady=0, padx=0, compound="center", command=reset)
    plotterConnectButton = tk.Button(master=plotterUIButtonFrame, text="Disconnect", font=BUTTONFONT, height=buh, width=buw,
                            image=pixel, pady=0, padx=0, compound="center", command=portConnect, fg='red')

    datWinXButton = tk.Button(master=dataWindowHeaderSelectFrame, text='X', command=datWinX, font=BUTTONFONT,
                              height=12, width=12, image=pixel, compound="center")

    plotButton = tk.Button(master=plotterPlotSelectUIFrame, text='Plot', command=plot, font=LARGEFONT)
    
        #generalRadioButton = tk.Radiobutton(master=generalRadioButtonFrame, text="General", bg=color, font=MEDFONT, variable=bu1, value=0, activebackground=color)
    monitorButton = tk.Radiobutton(master=headerFrame, text="  Serial Monitor  ", font=MEDFONT, variable=page,
                                   value=0, pady=0, padx=0, command=monitor, indicatoron=0)
    plotterButton = tk.Radiobutton(master=headerFrame, text="  Serial Plotter  ", font=MEDFONT, variable=page,
                                   value=1, pady=0, padx=0, command=plotter, indicatoron=0)

    datSerialButton = tk.Radiobutton(master=dataWindowHeaderSelectFrame, text="Serial Monitor", font=BUTTONFONT, variable=datSel,
                                  value=6, command=datPrint, indicatoron=0, width=14)
    datAllButton = tk.Radiobutton(master=dataWindowHeaderSelectFrame, text="All Data", font=BUTTONFONT, variable=datSel,
                                  value=7, command=datPrint, indicatoron=0, width=8)
    dat1Button = tk.Radiobutton(master=dataWindowHeaderSelectFrame, textvariable=input1, font=BUTTONFONT, variable=datSel,
                                  value=1, command=datPrint, indicatoron=0, padx=3)#width=6)
    dat2Button = tk.Radiobutton(master=dataWindowHeaderSelectFrame, textvariable=input2, font=BUTTONFONT, variable=datSel,
                                  value=2, command=datPrint, indicatoron=0, width=6)
    dat3Button = tk.Radiobutton(master=dataWindowHeaderSelectFrame, textvariable=input3, font=BUTTONFONT, variable=datSel,
                                  value=3, command=datPrint, indicatoron=0, width=6)
    dat4Button = tk.Radiobutton(master=dataWindowHeaderSelectFrame, textvariable=input4, font=BUTTONFONT, variable=datSel,
                                  value=4, command=datPrint, indicatoron=0, width=6)
    dat5Button = tk.Radiobutton(master=dataWindowHeaderSelectFrame, textvariable=input5, font=BUTTONFONT, variable=datSel,
                                  value=5, command=datPrint, indicatoron=0, width=6)

    levelPlotButton = tk.Radiobutton(master=plotterCalPropRadioFrame,variable=calPlotType,text='Default Calibration Curve',value=0,font=BUTTONFONT,command=calPlotConfig)
    boxPlotButton = tk.Radiobutton(master=plotterCalPropRadioFrame,variable=calPlotType,text='Box Plot',value=1,font=BUTTONFONT,command=calPlotConfig)
    regAnlyPlotButton = tk.Radiobutton(master=plotterCalPropRadioFrame,variable=calPlotType,text='Regression Analysis',value=2,font=BUTTONFONT,command=calPlotConfig)
    pwDistPlotButton = tk.Radiobutton(master=plotterCalPropRadioFrame,variable=calPlotType,text='Piecewise Distribution',value=3,font=BUTTONFONT,command=calPlotConfig)
    
    xNButton = tk.Radiobutton(master=plotterCalXTopFrame, variable=xType, value=0, font=BUTTONFONT, command=xConfig, text='Element [n]')
    xPButton = tk.Radiobutton(master=plotterCalXTopFrame, variable=xType, value=1, font=BUTTONFONT, command=xConfig, text='Physical Value')
    yNButton = tk.Radiobutton(master=plotterCalYTopFrame, variable=yType, value=0, font=BUTTONFONT, command=yConfig, text='Raw Decimal')
    yPButton = tk.Radiobutton(master=plotterCalYTopFrame, variable=yType, value=1, font=BUTTONFONT, command=yConfig, text='Physical Value')
    yCustomButton = tk.Radiobutton(master=plotterCalYButton2Frame, variable=yTypeB, value=0, font=BUTTONFONT, command=yConfig, text='Custom')
    yADCButton = tk.Radiobutton(master=plotterCalYButton2Frame, variable=yTypeB, value=1, font=BUTTONFONT, command=yConfig, text='ADC Conversion')
    
        #generalBox = tk.Checkbutton(master=generalBoxFrame, text="General", variable=bx1, bg=color, activebackground=color)
    autoscrollBox = tk.Checkbutton(master=boxFrame, text="Autoscroll", variable=auto)
    timestampBox = tk.Checkbutton(master=boxFrame, text="Show Timestamp", variable=ts)
    dataWindowBox = tk.Checkbutton(master=plotterVariableBoxFrame, text="Show Data Window", variable=datWin,
                                   command=showData)
    plotWindowBox = tk.Checkbutton(master=plotterVariableBoxFrame, text="Show Plot", variable=plotWin,
                                   command=showPlot)
    variableWindowBox = tk.Checkbutton(master=plotterUIFrame, text="Show Variable Frame", variable=varWin,
                                       command=showVar)
    calInvBox = tk.Checkbutton(master=plotterCalPropBoxFrame, text="Invert Plot", variable=invPlot, command=plot)
    calRawBox = tk.Checkbutton(master=plotterCalPropBoxFrame, text="Raw Data", variable=rawAct, command=plot)
    calAveBox = tk.Checkbutton(master=plotterCalPropBoxFrame, text="Averages", variable=aveAct, command=plot)
    calTrendBox = tk.Checkbutton(master=plotterCalPropBoxFrame, text="Trendline", variable=trendAct, command=plot)
    calLegendBox = tk.Checkbutton(master=plotterCalPropBoxFrame, text="Legend", variable=legAct, command=plot)
    calActualBox = tk.Checkbutton(master=plotterCalPropBoxFrame, text="Actual Data", variable=realAct, command=plot)

    xMassBox = tk.Checkbutton(master=plotterCalXBoxFrame, variable=xTypeA, font=BUTTONFONT, command=xConfig, text='Measured Mass Set')
    yRatioBox = tk.Checkbutton(master=plotterCalYButton2Frame, variable=yTypeA, font=BUTTONFONT, command=yConfig, text='Full Scale Ratio')
    
        #generalMenu = tk.OptionMenu(master...)
    plotterMenu = tk.Menubutton(master=plotterPlotSelectUIFrame, text="Plot Type", relief=tk.RAISED)
    plotterMenu.menu = tk.Menu(master=plotterMenu, tearoff=0)
    plotterMenu["menu"] = plotterMenu.menu
    plotterMenu.menu.add_radiobutton(label="y = f(x)", variable=module, value=0, command=setPlot)
    plotterMenu.menu.add_radiobutton(label="Live Plot", variable=module, value=1, command=setPlot)
    plotterMenu.menu.add_radiobutton(label="Calibration Curve", variable=module, value=2, command=setPlot)
    plotterMenu.menu.add_radiobutton(label="FFT", variable=module, value=3, command=setPlot)
        
    #---------------Page Formatting---------------------------------------------------------------------------

        #generalFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=110)
    body.pack(fill=tk.BOTH, expand=True)
    headerFrame.pack(side=tk.TOP, fill=tk.BOTH)
    monitorFrame.pack(side=tk.BOTTOM, fill=tk.BOTH)
    uiFrame.pack(side=tk.BOTTOM, fill=tk.BOTH,expand=True)
    boxFrame.pack(side=tk.LEFT, fill=tk.BOTH, padx=10)
    uiButtonFrame.pack(side=tk.RIGHT, fill=tk.BOTH, padx=10)
    uiIndicatorFrame.pack(side=tk.RIGHT, fill=tk.BOTH)
    uiIndicatorLabelFrame.pack(side=tk.RIGHT, fill=tk.BOTH)
    printFrame.pack(side=tk.TOP, fill=tk.BOTH)

    plotterUIFrame.pack(side=tk.BOTTOM, fill=tk.BOTH)
    plotterUIButtonFrame.pack(side=tk.RIGHT, fill=tk.BOTH, padx=10)
    plotterUIIndicatorFrame.pack(side=tk.RIGHT, fill=tk.BOTH)
    plotterUIIndicatorLabelFrame.pack(side=tk.RIGHT, fill=tk.BOTH)
    plotterDisplayFrame.pack(side=tk.TOP, fill=tk.BOTH, expand=True)
    plotterVariableFrame.pack(side=tk.BOTTOM, fill=tk.BOTH)
    plotterVariableBoxFrame.pack(side=tk.LEFT, fill=tk.BOTH)#, expand=True)
    plotterVariableNameFrame.pack(side=tk.LEFT, fill=tk.BOTH)#, expand=True)
    plotterPlotSelectFrame.pack(side=tk.RIGHT, fill=tk.BOTH)#, expand=True)
    plotterPlotSelectTextFrame.pack(side=tk.TOP, fill=tk.BOTH)#, expand=True)
    plotterPlotSelectUIFrame.pack(side=tk.TOP, fill=tk.BOTH)#, expand=True)
    plotterVariableEntryFrame.pack(side=tk.BOTTOM, fill=tk.Y, pady=15, anchor='w')
    plotterVariableSelectFrame.pack(side=tk.BOTTOM, fill=tk.Y, anchor='w', pady=15)

    plotterCalPropRadioFrame.pack(side=tk.LEFT,fill=tk.BOTH,expand=True)
    plotterCalPropBoxFrame.pack(side=tk.LEFT,fill=tk.BOTH,expand=True)
    
    plotterCalXFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    plotterCalYFrame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
    
    plotterCalXTitleFrame.pack(side=tk.TOP,anchor='n',padx=200)#fill=tk.X,expand=True,
    plotterCalXBodyFrame.pack(side=tk.TOP,fill=tk.BOTH,expand=True,anchor='n')
    plotterCalXTopFrame.pack(side=tk.TOP,fill=tk.BOTH,expand=True)
    plotterCalXBotFrame.pack(side=tk.BOTTOM,fill=tk.BOTH,expand=True,pady=15)
    plotterCalXBoxFrame.pack(side=tk.LEFT,anchor='nw')
    plotterCalXEntryFrame.pack(anchor='nw')
    plotterCalXLabelFrame.pack(anchor='e',padx=10)
    plotterCalXScalarFrame.pack(anchor='e',padx=10)
    
    plotterCalYTitleFrame.pack(side=tk.TOP,anchor='n',padx=200)
    plotterCalYBodyFrame.pack(side=tk.TOP,fill=tk.BOTH,expand=True,anchor='n')
    plotterCalYTopFrame.pack(side=tk.TOP,fill=tk.BOTH,expand=True)
    plotterCalYBotFrame.pack(side=tk.BOTTOM,fill=tk.BOTH,expand=True)
    plotterCalYButton2Frame.pack(side=tk.LEFT,anchor='nw')
    plotterCalYEntryFrame.pack(anchor='nw')
    plotterCalYLabelFrame.pack(anchor='e',padx=10)
    plotterCalYScalarFrame.pack(anchor='e',padx=10)
    plotterCalYOffsetFrame.pack(anchor='e',padx=10)

    dataWindowHeaderFrame.pack(side=tk.TOP, fill=tk.X)#, expand=True)
    dataWindowPrintFrame.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)
    dataWindowHeaderSelectFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    #dataWindowXButtonFrame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)

        #generalLabel.pack(fill=tk.BOTH, padx=10, side=tk.LEFT, expand=True, pady=15)
    indicatorLabel.pack(side=tk.RIGHT)
    indicator.pack(side=tk.LEFT)

    plotterIndicatorLabel.pack(side=tk.RIGHT)
    plotterIndicator.pack(side=tk.LEFT)

    variableIDLabel.pack(side=tk.LEFT, padx=10)

    moduleLabel.pack(side=tk.TOP, pady=10)

    variableSelectLabel.pack(side=tk.LEFT, padx=10, pady=10)

    plotterCalXLabel.pack()
    plotterCalXLabelLabel.pack(side=tk.LEFT)
    plotterCalXScalarLabel.pack(side=tk.LEFT)
    plotterCalYLabel.pack()
    plotterCalYLabelLabel.pack(side=tk.LEFT)
    plotterCalYScalarLabel.pack(side=tk.LEFT)
    plotterCalYOffsetLabel.pack(side=tk.LEFT)

        #generalScroll.pack()
    textScroll.pack(side=tk.RIGHT, fill=tk.Y)
    dataWindowScroll.pack(side=tk.RIGHT, fill=tk.Y)

        #generalEntry.pack()
    xLabelEntry.pack(side=tk.RIGHT)
    xScalarEntry.pack(side=tk.RIGHT)
    yLabelEntry.pack(side=tk.RIGHT)
    yScalarEntry.pack(side=tk.RIGHT)
    yOffsetEntry.pack(side=tk.RIGHT)

        #outputCanvas.pack(fill=tk.BOTH, expand=True)
    #plotterCanvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    screen.pack(side=tk.LEFT,fill=tk.BOTH,expand=True)
    dataScreen.pack(side=tk.LEFT,fill=tk.BOTH,expand=True)

        #generalButton.pack(side=tk.LEFT, padx=20)
    monitorButton.pack(side=tk.LEFT, pady=5, padx=10)
    plotterButton.pack(side=tk.LEFT, pady=5)

    datWinXButton.pack(side=tk.RIGHT, anchor='ne',padx=1)#, expand=True)
    
    connectButton.pack(side=tk.LEFT, pady=2, padx=5)
    resetButton.pack(side=tk.LEFT, pady=2, padx=5)
    clearButton.pack(side=tk.LEFT, pady=2, padx=5)

    plotterConnectButton.pack(side=tk.LEFT, pady=2, padx=5)
    plotterResetButton.pack(side=tk.LEFT, pady=2, padx=5)
    plotterClearButton.pack(side=tk.LEFT, pady=2, padx=5)

    datSerialButton.pack(side=tk.LEFT)

    #datAllButton.pack(side=tk.LEFT)
    
        #generalRadioButton.grid(row=0,column=0,sticky="w")
    levelPlotButton.pack(anchor='w')
    boxPlotButton.pack(anchor='w')
    regAnlyPlotButton.pack(anchor='w')
    pwDistPlotButton.pack(anchor='w')
    xNButton.pack(anchor='w')
    yNButton.pack(anchor='w')
    xPButton.pack(anchor='w')
    yPButton.pack(anchor='w')
    yCustomButton.pack(anchor='w', padx=20)
    yADCButton.pack(anchor='w', padx=20)
    
        #generalBox.grid(row=0,column=0,sticky="w")
    autoscrollBox.pack(side=tk.LEFT, pady=2, padx=5)
    timestampBox.pack(side=tk.RIGHT, pady=2, padx=5)
    dataWindowBox.pack(side=tk.TOP, pady=10, padx=20, anchor='nw')
    variableWindowBox.pack(side=tk.LEFT, anchor='w',pady=2, padx=5)

    calInvBox.pack(anchor='w')
    calRawBox.pack(anchor='w')
    calAveBox.pack(anchor='w')
    calTrendBox.pack(anchor='w')
    calLegendBox.pack(anchor='w')
    calActualBox.pack(anchor='w')
    xMassBox.pack(anchor='nw',padx=20)

    plotterMenu.pack(side=tk.RIGHT,pady=40,padx=60)

    #---------------

    print('Starting Serial Monitor\n')
    setup()
    main()
    #window.mainloop()

mainHandler()

print('\nExited Serial Monitor')
