import serial 
import argparse
import tkinter as tk
from tkinter import ttk, Tk, Label, Button, Frame

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

    test = "Peepee\n\tand\nPoopoo\n"
    
    pixel = tk.PhotoImage(width=1, height=1)

    #---------------Page Variables

        #generalString = tk.StringVar(master=window)
    lines = tk.StringVar(master=window)
    printOn = tk.StringVar(master=window, value="disabled")
    
        #generalInt = tk.IntVar(master=window, value=0)
    status = tk.BooleanVar(master=window, value=True)
    auto = tk.IntVar(master=window, value=1)
    ts = tk.IntVar(master=window, value=0)
    read = tk.IntVar(master=window, value=0)
    rst = tk.IntVar(master=window, value=0)
    page = tk.IntVar(master=window, value=0)

        #generalDouble = tk.DoubleVar(master=window, value=0.0)
    y0 = tk.DoubleVar(master=window, value=0.0)
    y1 = tk.DoubleVar(master=window, value=0.0)

    #---------------Page Commands

    def main():
        while True:
            if status.get():
                if not serial_port.inWaiting()==0:
                    lines.set(serial_port.readline().decode())
                    
                    #print(serial_port.inWaiting())
                    #y0.set(screen.yview()[0])
                    #y1.set(screen.yview()[1])
                    #print(y0.get(),'\t',y1.get())
                    screen.insert(tk.INSERT,lines.get())
                    if auto.get():
                        screen.see("end")
                    #print('\t',textScroll.get())
            try:
                textScroll.config(command = screen.yview)
                window.update_idletasks()
                window.update()
            except:
                break

    def reset():
        screen.config(state=tk.NORMAL)
        screen.insert(tk.INSERT, "\n\n===== RESET SERIAL PORT: "+port+" =====\n\n")
        #screen.config(state=tk.DISABLED)
        serial_port.close()
        serial_port.open()
        status.set(True)
        connectButton.configure(text='Disconnect', fg='red')
        screen.insert(tk.INSERT, "\n\n===== SERIAL PORT CONNECTED =====\n\n")
        read.set(1)
        print(f"The Port name is {serial_port.name}")

    def clear():
        screen.config(state=tk.NORMAL)
        screen.delete(1.0,tk.END)
        screen.config(state=tk.DISABLED)

    def portConnect():
        status.set(not status.get())
        if status.get():
            serial_port.open()
            connectButton.configure(text='Disconnect', fg='red')
            screen.insert(tk.INSERT, "\n\n===== SERIAL PORT CONNECTED =====\n\n")
        else:
            serial_port.close()
            connectButton.configure(text='Connect', fg='green')
            screen.insert(tk.INSERT, "\n\n===== SERIAL PORT DISCONNECTED =====\n\n")

    #----------------Page Widgets

        #generalFrame = tk.Frame(master=window, bg=color, relief=border, borderwidth=bw)
    body = tk.Frame(master=window, relief=border, borderwidth=bw)#, bg=color)
    monitorFrame = tk.Frame(master=body, relief=border, borderwidth=bw)
    uiFrame = tk.Frame(master=monitorFrame, relief=border, borderwidth=bw)
    headerFrame = tk.Frame(master=body,relief=border, borderwidth=bw)
    boxFrame = tk.Frame(master=uiFrame)#, relief=border, borderwidth=bw)
    uiButtonFrame = tk.Frame(master=uiFrame)#, relief=border, borderwidth=bw)
    headerButtonFrame = tk.Frame(master=headerFrame, relief=border, borderwidth=bw)
    printFrame = tk.Frame(master=monitorFrame, relief=border, borderwidth=bw)
    
        #generalLabel = tk.Label(master=generalLabelFrame, text="General", bg=color, font=MEDFONT, relief=border)

        #generalCanvas = tk.Canvas(master=generalCanvasFrame)

        #generalScroll = tk.Scrollbar(master=generalScrollFrame, bg=color, variable=sc1)
    textScroll = tk.Scrollbar(master=printFrame, orient=tk.VERTICAL)#, command=scroll)

        #generalText = tk.Text(master=generalTextFrame)
    screen = tk.Text(master=printFrame, font=BUTTONFONT, state=tk.NORMAL, height=64,
                     yscrollcommand=textScroll.set)

        #generalButton = tk.Button(master=generalButtonFrame, text="General", command=buttonPush, font=LARGEFONT)
    monitorButton = tk.Radiobutton(master=headerFrame, text="  Serial Monitor  ", font=MEDFONT, variable=page,
                                   value=0, pady=0, padx=0, command=clear, indicatoron=0)
    plotterButton = tk.Radiobutton(master=headerFrame, text="  Live Plotter  ", font=MEDFONT, variable=page,
                                   value=1, pady=0, padx=0, command=clear, indicatoron=0)

    clearButton = tk.Button(master=uiButtonFrame, text="Clear Output", font=BUTTONFONT, height=buh, width=buw,
                            image=pixel, pady=0, padx=0, compound="center", command=clear)
    resetButton = tk.Button(master=uiButtonFrame, text="Reset Port", font=BUTTONFONT, height=buh, width=buw,
                            image=pixel, pady=0, padx=0, compound="center", command=reset)
    connectButton = tk.Button(master=uiButtonFrame, text="Disconnect", font=BUTTONFONT, height=buh, width=buw,
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
    printFrame.pack(side=tk.TOP, fill=tk.BOTH)

        #generalLabel.pack(fill=tk.BOTH, padx=10, side=tk.LEFT, expand=True, pady=15)

        #generalScroll.pack()
    textScroll.pack(side=tk.RIGHT, fill=tk.Y)

        #outputCanvas.pack(fill=tk.BOTH, expand=True)

    screen.pack(side=tk.LEFT,fill=tk.BOTH,expand=True)#, expand=True)

        #generalButton.pack(side=tk.LEFT, padx=20)
    monitorButton.pack(side=tk.LEFT, pady=5, padx=10)
    plotterButton.pack(side=tk.LEFT, pady=5, padx=10)
    
    #clearButton.pack(side=tk.RIGHT, pady=2, padx=5)
    #resetButton.pack(side=tk.LEFT, pady=2, padx=5)
    connectButton.pack(side=tk.LEFT, pady=2, padx=5)
    resetButton.pack(side=tk.LEFT, pady=2, padx=5)
    clearButton.pack(side=tk.LEFT, pady=2, padx=5)
    
        #generalRadioButton.grid(row=0,column=0,sticky="w")
    
        #generalBox.grid(row=0,column=0,sticky="w")
    autoscrollBox.pack(side=tk.LEFT, pady=2, padx=5)
    timestampBox.pack(side=tk.RIGHT, pady=2, padx=5)

    #---------------

    print('Starting Serial Monitor\n')
    main()
    #window.mainloop()
    
serial_monitor()

print('\nExited Serial Monitor')
