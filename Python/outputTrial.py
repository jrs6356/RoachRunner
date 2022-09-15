import serial 
import argparse
import tkinter as tk
from tkinter import ttk, Tk, Label, Button, Frame
from PIL import Image, ImageTk
import PIL
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, 
NavigationToolbar2Tk)

def serial_monitor():
    window = tk.Tk()

    #--------------- Page Settings

    port = 'COM6'
    serial_port = serial.Serial(port,115200)

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

    #---------------Page Variables

        #generalString = tk.StringVar(master=window)
    lines = tk.StringVar(master=window)
    indText = tk.StringVar(master=window, value=port+': No Connection')
    input1 = tk.StringVar(master=window)
    input2 = tk.StringVar(master=window)
    input3 = tk.StringVar(master=window)
    input4 = tk.StringVar(master=window)
    input5 = tk.StringVar(master=window)
    par1 = tk.StringVar(master=window)
    par2 = tk.StringVar(master=window)
    
        #generalInt = tk.IntVar(master=window, value=0)
    status = tk.BooleanVar(master=window, value=True)
    auto = tk.IntVar(master=window, value=1)
    ts = tk.IntVar(master=window, value=0)  #timestamp
    read = tk.IntVar(master=window, value=0)
    rst = tk.IntVar(master=window, value=0)
    page = tk.IntVar(master=window, value=0)

        #generalDouble = tk.DoubleVar(master=window, value=0.0)
    y0 = tk.DoubleVar(master=window, value=0.0)
    y1 = tk.DoubleVar(master=window, value=0.0)

    #---------------Page Commands
    def setup():
        print('Setup')
        try:
            window.update_idletasks()
            window.update()
            screenPrint("\n\n===== SERIAL PORT CONNECTED =====\n\n")
            indicator.configure(image = onInd)
            indicator.image = onInd
            plotterIndicator.configure(image = onInd)
            plotterIndicator.image = onInd
            indText.set(port+': Active')
            status.set(True)
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
                textScroll.config(command = screen.yview)
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
        print('Screen Print')
        screen.config(state=tk.NORMAL)
        screen.insert(tk.INSERT, out)
        if auto.get():
            screen.see("end")
        screen.config(state=tk.DISABLED)

    def monitor():
        print('Monitor')
        plotterFrame.pack_forget()
        monitorFrame.pack(side=tk.BOTTOM, fill=tk.BOTH)
        print(status.get())

    def plotter():
        print('Plotter')
        monitorFrame.pack_forget()
        plotterFrame.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)
        print(status.get())

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
    plotterVariableEntryFrame       = tk.Frame(master=plotterVariableFrame, relief=border, borderwidth=bw)
    plotterVariableSelectFrame      = tk.Frame(master=plotterVariableFrame, relief=border, borderwidth=bw)
    plotterDataWindowFrame          = tk.Frame(master=plotterDisplayFrame, relief=border, borderwidth=bw)
    
    
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

        #generalScroll = tk.Scrollbar(master=generalScrollFrame, bg=color, variable=sc1)
    textScroll              = tk.Scrollbar(master=printFrame, orient=tk.VERTICAL)

        #generalEntry = tk.Entry(master=generalEntryFrame, textvariable=strvar1, command=readtext)
    plotterEntry1           = tk.Entry(master=plotterVariableEntryFrame, textvariable=input1, font=MEDFONT, width=5, relief=border)
    plotterEntry2           = tk.Entry(master=plotterVariableEntryFrame, textvariable=input2, font=MEDFONT, width=5, relief=border)
    plotterEntry3           = tk.Entry(master=plotterVariableEntryFrame, textvariable=input3, font=MEDFONT, width=5, relief=border)
    plotterEntry4           = tk.Entry(master=plotterVariableEntryFrame, textvariable=input4, font=MEDFONT, width=5, relief=border)
    plotterEntry5           = tk.Entry(master=plotterVariableEntryFrame, textvariable=input5, font=MEDFONT, width=5, relief=border)

    plotterSelect1           = tk.Entry(master=plotterVariableSelectFrame, textvariable=par1, font=MEDFONT, width=5, relief=border)
    plotterSelect2           = tk.Entry(master=plotterVariableSelectFrame, textvariable=par2, font=MEDFONT, width=5, relief=border)

        #generalText = tk.Text(master=generalTextFrame)
    screen = tk.Text(master=printFrame, font=BUTTONFONT, state=tk.NORMAL, height=64,
                     yscrollcommand=textScroll.set)

        #generalButton = tk.Button(master=generalButtonFrame, text="General", command=buttonPush, font=LARGEFONT)
    monitorButton = tk.Radiobutton(master=headerFrame, text="  Serial Monitor  ", font=MEDFONT, variable=page,
                                   value=0, pady=0, padx=0, command=monitor, indicatoron=0)
    plotterButton = tk.Radiobutton(master=headerFrame, text="  Live Plotter  ", font=MEDFONT, variable=page,
                                   value=1, pady=0, padx=0, command=plotter, indicatoron=0)

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
    
        #generalRadioButton = tk.Radiobutton(master=generalRadioButtonFrame, text="General", bg=color, font=MEDFONT, variable=bu1, value=0, activebackground=color)

        #generalBox = tk.Checkbutton(master=generalBoxFrame, text="General", variable=bx1, bg=color, activebackground=color)
    autoscrollBox = tk.Checkbutton(master=boxFrame, text="Autoscroll", variable=auto)
    timestampBox = tk.Checkbutton(master=boxFrame, text="Show Timestamp", variable=ts)
        
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
    plotterVariableEntryFrame.pack(side=tk.BOTTOM, fill=tk.Y, pady=15)
    plotterVariableSelectFrame.pack(side=tk.BOTTOM, fill=tk.Y, pady=15)
    plotterDataWindowFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    plotterCanvasFrame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)

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

        #outputCanvas.pack(fill=tk.BOTH, expand=True)
    plotterCanvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    screen.pack(side=tk.LEFT,fill=tk.BOTH,expand=True)#, expand=True)

        #generalButton.pack(side=tk.LEFT, padx=20)
    monitorButton.pack(side=tk.LEFT, pady=5, padx=10)
    plotterButton.pack(side=tk.LEFT, pady=5, padx=10)
    
    #clearButton.pack(side=tk.RIGHT, pady=2, padx=5)
    #resetButton.pack(side=tk.LEFT, pady=2, padx=5)
    connectButton.pack(side=tk.LEFT, pady=2, padx=5)
    resetButton.pack(side=tk.LEFT, pady=2, padx=5)
    clearButton.pack(side=tk.LEFT, pady=2, padx=5)

    plotterConnectButton.pack(side=tk.LEFT, pady=2, padx=5)
    plotterResetButton.pack(side=tk.LEFT, pady=2, padx=5)
    plotterClearButton.pack(side=tk.LEFT, pady=2, padx=5)
    
        #generalRadioButton.grid(row=0,column=0,sticky="w")
    
        #generalBox.grid(row=0,column=0,sticky="w")
    autoscrollBox.pack(side=tk.LEFT, pady=2, padx=5)
    timestampBox.pack(side=tk.RIGHT, pady=2, padx=5)

    #---------------

    print('Starting Serial Monitor\n')
    setup()
    main()
    #window.mainloop()
    
serial_monitor()

print('\nExited Serial Monitor')
