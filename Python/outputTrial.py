import serial 
import argparse
import tkinter as tk
from tkinter import ttk, Tk, Label, Button, Frame
from PIL import Image, ImageTk
import PIL
import csv
import pandas as pd
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

    #--------------- Page Settings

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
    w = (1920/2)-1
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
    df = pd.DataFrame()
    v1 = []
    v2 = []
    v3 = []
    v4 = []
    v5 = []

    #---------------Page Variables

        #generalString = tk.StringVar(master=window)
    lines = tk.StringVar(master=window)
    indText = tk.StringVar(master=window, value=port+': No Connection')
    datID = tk.StringVar(master=window)
    input1 = tk.StringVar(master=window)
    input2 = tk.StringVar(master=window)
    input3 = tk.StringVar(master=window)
    input4 = tk.StringVar(master=window)
    input5 = tk.StringVar(master=window)
    par1 = tk.StringVar(master=window)
    par2 = tk.StringVar(master=window)
    
        #generalBoolean = tk.BooleanVar(master=window, value=False)
    status = tk.BooleanVar(master=window, value=True)
    datWin = tk.BooleanVar(master=window, value=True)
    datAct = tk.BooleanVar(master=window, value=False)
    datRead = tk.BooleanVar(master=window, value=False)
    datWrite = tk.BooleanVar(master=window, value=False)
    datReady = tk.BooleanVar(master=window, value=False)

        #generalInt = tk.IntVar(master=window, value=0)
    auto = tk.IntVar(master=window, value=1)
    ts = tk.IntVar(master=window, value=0)  #timestamp
    datRead = tk.IntVar(master=window, value=0)
    rst = tk.IntVar(master=window, value=0)
    page = tk.IntVar(master=window, value=1)
    datSel = tk.IntVar(master=window, value=6)
    index = tk.IntVar(master=window, value=0)
    nPars = tk.IntVar(master=window, value=0)
    plotType = tk.IntVar(master=window, value=0)

        #generalDouble = tk.DoubleVar(master=window, value=0.0)
    y0 = tk.DoubleVar(master=window, value=0.0)
    y1 = tk.DoubleVar(master=window, value=0.0)

    #---------------Page Commands
    def setup():
        print('Setup')
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
                if not serial_port.inWaiting()==0:
                    lines.set(serial_port.readline().decode())
                    screenPrint(lines.get())
            try:
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
            serFile.truncate(0)
            #serCSV.truncate(0)
            datAllButton.pack_forget()
        elif "#END" in out:
            datAct.set(False)
            print("Data Collected")
            logCSV()
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

    def setPlot():
        print('ayyyy imma plot was goin on')

    def showData():
        if datWin.get():
            plotterDataWindowFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        else:
            plotterDataWindowFrame.pack_forget()

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
        data = serFile.readlines()
        datID.set(data[0].rstrip())
        print(datID.get())
        nPars.set(data[0].count(',')+1)
        print(nPars.get())
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

    def datPrint():
        #print('Screen Print')
        dataScreen.delete(1.0, tk.END)

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

    #----------------Page Widgets

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
    plotterVariableNameFrame        = tk.Frame(master=plotterVariableFrame, relief=border, borderwidth=bw)
    plotterVariableEntryFrame       = tk.Frame(master=plotterVariableNameFrame)#, relief=border, borderwidth=bw)
    plotterVariableSelectFrame      = tk.Frame(master=plotterVariableNameFrame)#, relief=border, borderwidth=bw)
    plotterVariableBoxFrame         = tk.Frame(master=plotterVariableFrame, relief=border, borderwidth=bw)
    plotterPlotSelectFrame          = tk.Frame(master=plotterVariableFrame, relief=border, borderwidth=bw)
    plotterDataWindowFrame          = tk.Frame(master=plotterDisplayFrame, relief=border, borderwidth=bw)

    dataWindowHeaderFrame           = tk.Frame(master=plotterDataWindowFrame, relief=border, borderwidth=bw)
    dataWindowPrintFrame            = tk.Frame(master=plotterDataWindowFrame, relief=border, borderwidth=bw)
    dataWindowHeaderSelectFrame     = tk.Frame(master=dataWindowHeaderFrame)#, relief=border, borderwidth=bw)
    
        #generalLabel = tk.Label(master=generalLabelFrame, text="General", bg=color, font=MEDFONT, relief=border)
    indicator               = tk.Label(master=uiIndicatorFrame, image=offInd)
    indicatorLabel          = tk.Label(master=uiIndicatorLabelFrame, textvariable=indText, font=BUTTONFONT, width=18, anchor='w')

    plotterIndicator        = tk.Label(master=plotterUIIndicatorFrame, image=offInd)
    plotterIndicatorLabel   = tk.Label(master=plotterUIIndicatorLabelFrame, textvariable=indText, font=BUTTONFONT, width=18, anchor='w')

    variableIDLabel         = tk.Label(master=plotterVariableEntryFrame, text='Input Data:', font=LARGEFONT)
    variableSpacerLabel1    = tk.Label(master=plotterVariableEntryFrame, text=',', font=LARGEFONT)
    variableSpacerLabel2    = tk.Label(master=plotterVariableEntryFrame, text=',', font=LARGEFONT)
    variableSpacerLabel3    = tk.Label(master=plotterVariableEntryFrame, text=',', font=LARGEFONT)
    variableSpacerLabel4    = tk.Label(master=plotterVariableEntryFrame, text=',', font=LARGEFONT)

    variableSelectLabel     = tk.Label(master=plotterVariableSelectFrame, text='Plot Data:', font=LARGEFONT)
    variableSelectSpacer    = tk.Label(master=plotterVariableSelectFrame, text='vs', font=LARGEFONT)
    
        #generalCanvas = tk.Canvas(master=generalCanvasFrame)
    plotterCanvas           = tk.Canvas(master=plotterCanvasFrame, bg=color)
    #plotterCanvas           = FigureCanvasTkAgg(master=plotterCanvasFrame)

        #generalScroll = tk.Scrollbar(master=generalScrollFrame, bg=color, variable=sc1)
    textScroll              = tk.Scrollbar(master=printFrame, orient=tk.VERTICAL)
    dataWindowScroll        = tk.Scrollbar(master=dataWindowPrintFrame, orient=tk.VERTICAL)

        #generalEntry = tk.Entry(master=generalEntryFrame, textvariable=strvar1, command=readtext)
    plotterEntry1           = tk.Entry(master=plotterVariableEntryFrame, textvariable=input1, font=MEDFONT, width=5, bd=4)
    plotterEntry2           = tk.Entry(master=plotterVariableEntryFrame, textvariable=input2, font=MEDFONT, width=5, bd=4)
    plotterEntry3           = tk.Entry(master=plotterVariableEntryFrame, textvariable=input3, font=MEDFONT, width=5, bd=4)
    plotterEntry4           = tk.Entry(master=plotterVariableEntryFrame, textvariable=input4, font=MEDFONT, width=5, bd=4)
    plotterEntry5           = tk.Entry(master=plotterVariableEntryFrame, textvariable=input5, font=MEDFONT, width=5, bd=4)
##    plotterEntry1.bind('<Any-KeyRelease>',addTab)
##    plotterEntry2.bind('<Any-KeyRelease>',addTab)
##    plotterEntry3.bind('<Any-KeyRelease>',addTab)
##    plotterEntry4.bind('<Any-KeyRelease>',addTab)
##    plotterEntry5.bind('<Any-KeyRelease>',addTab)

    plotterSelect1          = tk.Entry(master=plotterVariableSelectFrame, textvariable=par1, font=MEDFONT, width=5, bd=4)
    plotterSelect2          = tk.Entry(master=plotterVariableSelectFrame, textvariable=par2, font=MEDFONT, width=5, bd=4)
    plotterSelect1.bind('<Any-KeyRelease>',setPlot)
    plotterSelect2.bind('<Any-KeyRelease>',setPlot)

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

    datWinXButton = tk.Button(master=dataWindowHeaderFrame, text='X', command=datWinX, font=BUTTONFONT,
                              height=12, width=12, image=pixel, compound="center")

    plotButton = tk.Button(master=plotterVariableSelectFrame, text='Plot', command=setPlot, font=LARGEFONT)
    
        #generalRadioButton = tk.Radiobutton(master=generalRadioButtonFrame, text="General", bg=color, font=MEDFONT, variable=bu1, value=0, activebackground=color)
    monitorButton = tk.Radiobutton(master=headerFrame, text="  Serial Monitor  ", font=MEDFONT, variable=page,
                                   value=0, pady=0, padx=0, command=monitor, indicatoron=0)
    plotterButton = tk.Radiobutton(master=headerFrame, text="  Live Plotter  ", font=MEDFONT, variable=page,
                                   value=1, pady=0, padx=0, command=plotter, indicatoron=0)

    datSerialButton = tk.Radiobutton(master=dataWindowHeaderSelectFrame, text="Serial Monitor", font=BUTTONFONT, variable=datSel,
                                  value=6, command=datPrint, indicatoron=0, width=14)
    datAllButton = tk.Radiobutton(master=dataWindowHeaderSelectFrame, text="All Data", font=BUTTONFONT, variable=datSel,
                                  value=7, command=datPrint, indicatoron=0, width=8)
    dat1Button = tk.Radiobutton(master=dataWindowHeaderSelectFrame, textvariable=input1, font=BUTTONFONT, variable=datSel,
                                  value=1, command=datPrint, indicatoron=0, width=6)
    dat2Button = tk.Radiobutton(master=dataWindowHeaderSelectFrame, textvariable=input2, font=BUTTONFONT, variable=datSel,
                                  value=2, command=datPrint, indicatoron=0, width=6)
    dat3Button = tk.Radiobutton(master=dataWindowHeaderSelectFrame, textvariable=input3, font=BUTTONFONT, variable=datSel,
                                  value=3, command=datPrint, indicatoron=0, width=6)
    dat4Button = tk.Radiobutton(master=dataWindowHeaderSelectFrame, textvariable=input4, font=BUTTONFONT, variable=datSel,
                                  value=4, command=datPrint, indicatoron=0, width=6)
    dat5Button = tk.Radiobutton(master=dataWindowHeaderSelectFrame, textvariable=input5, font=BUTTONFONT, variable=datSel,
                                  value=5, command=datPrint, indicatoron=0, width=6)

        #generalBox = tk.Checkbutton(master=generalBoxFrame, text="General", variable=bx1, bg=color, activebackground=color)
    autoscrollBox = tk.Checkbutton(master=boxFrame, text="Autoscroll", variable=auto)
    timestampBox = tk.Checkbutton(master=boxFrame, text="Show Timestamp", variable=ts)
    dataWindowBox = tk.Checkbutton(master=plotterVariableBoxFrame, text="Show Data Window", variable=datWin,
                                   command=showData)

        #generalMenu = tk.OptionMenu(master...)
    plotterMenu = tk.Menubutton(master=plotterPlotSelectFrame, text="Plot Type", relief=tk.RAISED)
    plotterMenu.menu = tk.Menu(master=plotterMenu, tearoff=0)
    plotterMenu["menu"] = plotterMenu.menu
    plotterMenu.menu.add_radiobutton(label="y = f(x)", variable=plotType, value=0)
    plotterMenu.menu.add_radiobutton(label="FFT", variable=plotType, value=1)
    plotterMenu.menu.add_radiobutton(label="Calibration Curve", variable=plotType, value=2)
    
        
    #---------------Page Formatting

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
    plotterVariableBoxFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    plotterVariableNameFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    plotterPlotSelectFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    plotterVariableEntryFrame.pack(side=tk.BOTTOM, fill=tk.Y, pady=15, anchor='w')
    plotterVariableSelectFrame.pack(side=tk.BOTTOM, fill=tk.Y, pady=15, anchor='w')
    plotterCanvasFrame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)

    dataWindowHeaderFrame.pack(side=tk.TOP, fill=tk.BOTH)
    dataWindowPrintFrame.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)
    dataWindowHeaderSelectFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        #generalLabel.pack(fill=tk.BOTH, padx=10, side=tk.LEFT, expand=True, pady=15)
    indicatorLabel.pack(side=tk.RIGHT)
    indicator.pack(side=tk.LEFT)

    plotterIndicatorLabel.pack(side=tk.RIGHT)
    plotterIndicator.pack(side=tk.LEFT)

    variableIDLabel.pack(side=tk.LEFT, padx=10)
    plotterEntry1.pack(side=tk.LEFT)
    variableSpacerLabel1.pack(side=tk.LEFT)
    plotterEntry2.pack(side=tk.LEFT)
    variableSpacerLabel2.pack(side=tk.LEFT)
    plotterEntry3.pack(side=tk.LEFT)
    variableSpacerLabel3.pack(side=tk.LEFT)
    plotterEntry4.pack(side=tk.LEFT)
    variableSpacerLabel4.pack(side=tk.LEFT)
    plotterEntry5.pack(side=tk.LEFT)

    variableSelectLabel.pack(side=tk.LEFT, padx=10)
    plotterSelect1.pack(side=tk.LEFT)
    variableSelectSpacer.pack(side=tk.LEFT)
    plotterSelect2.pack(side=tk.LEFT)

        #generalScroll.pack()
    textScroll.pack(side=tk.RIGHT, fill=tk.Y)
    dataWindowScroll.pack(side=tk.RIGHT, fill=tk.Y)

        #outputCanvas.pack(fill=tk.BOTH, expand=True)
    plotterCanvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    screen.pack(side=tk.LEFT,fill=tk.BOTH,expand=True)
    dataScreen.pack(side=tk.LEFT,fill=tk.BOTH,expand=True)

        #generalButton.pack(side=tk.LEFT, padx=20)
    monitorButton.pack(side=tk.LEFT, pady=5, padx=10)
    plotterButton.pack(side=tk.LEFT, pady=5)

    datWinXButton.pack(side=tk.RIGHT, anchor='ne')
    
    connectButton.pack(side=tk.LEFT, pady=2, padx=5)
    resetButton.pack(side=tk.LEFT, pady=2, padx=5)
    clearButton.pack(side=tk.LEFT, pady=2, padx=5)

    plotterConnectButton.pack(side=tk.LEFT, pady=2, padx=5)
    plotterResetButton.pack(side=tk.LEFT, pady=2, padx=5)
    plotterClearButton.pack(side=tk.LEFT, pady=2, padx=5)

    datSerialButton.pack(side=tk.LEFT)

    plotButton.pack(side=tk.LEFT, padx=20)
    #datAllButton.pack(side=tk.LEFT)
    
        #generalRadioButton.grid(row=0,column=0,sticky="w")
    
        #generalBox.grid(row=0,column=0,sticky="w")
    autoscrollBox.pack(side=tk.LEFT, pady=2, padx=5)
    timestampBox.pack(side=tk.RIGHT, pady=2, padx=5)
    dataWindowBox.pack(side=tk.LEFT, pady=10, padx=20, anchor='nw')

    plotterMenu.pack(pady=10)

    #---------------

    print('Starting Serial Monitor\n')
    setup()
    main()
    #window.mainloop()

mainHandler()

print('\nExited Serial Monitor')
