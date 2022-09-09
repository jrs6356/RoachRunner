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
    MEDFONT = 'Helvetica 16 italic'
    BUTTONFONT = 'Calibri 10'
    buh = 16
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
    
        #generalInt = tk.IntVar(master=window, value=0)
    scrollPos = tk.IntVar(master=window, value=0)
    bx1 = tk.IntVar(master=window, value=1)
    bx2 = tk.IntVar(master=window, value=0)
    read = tk.IntVar(master=window, value=0)

    #---------------Page Commands

    def update():
        #if read.get():
        #if serial_port.readable():
        lines.set(serial_port.readline())
        if lines.get() > 0:
            #print(serial_port.tell())
            print_serial()
        else:
            print("All is quiet")
        window.after(100,update)

    def reset():
        outputCanvas.insert(canvas_id, tk.END, "\n\n===== RESET SERIAL PORT: "+port+" =====\n\n")
        #serial_port = serial.Serial(port,115200)
        read.set(1)
        print(f"The Port name is {serial_port.name}")

    def clear():
        outputCanvas.itemconfigure(canvas_id, text="")

    def print_serial():
        try:
            #while True:
            #lines = serial_port.readline()
            #print(lines)
            outputCanvas.insert(canvas_id, tk.END, lines.get())
        except:
            print("ERROR")
            print("Check Port")
            read.set(0)

    #def scroll(event, a):
        #print(textScroll.get())
        #outputCanvas.create_text(text=textScroll.get(), font=BUTTONFONT)

    #----------------Page Widgets

        #generalFrame = tk.Frame(master=window, bg=color, relief=border, borderwidth=bw)
    body = tk.Frame(master=window, relief=border, borderwidth=bw)#, bg=color)
    uiFrame = tk.Frame(master=body, relief=border, borderwidth=bw)
    boxFrame = tk.Frame(master=uiFrame, relief=border, borderwidth=bw)
    buttonFrame = tk.Frame(master=uiFrame, relief=border, borderwidth=bw)
    printFrame = tk.Frame(master=body, relief=border, borderwidth=bw)
    textFrame = tk.Frame(master=printFrame, relief=border, borderwidth=bw)
    scrollerFrame = tk.Frame(master=printFrame, relief=border, borderwidth=bw)
    
        #generalLabel = tk.Label(master=generalLabelFrame, text="General", bg=color, font=MEDFONT, relief=border)

        #generalCanvas = tk.Canvas(master=generalCanvasFrame)
    outputCanvas = tk.Canvas(master=textFrame, bg='#FFFFFF')
    canvas_id = outputCanvas.create_text(5, 5, anchor=tk.NW, font=BUTTONFONT)

        #generalButton = tk.Button(master=generalButtonFrame, text="General", command=buttonPush, font=LARGEFONT)
    clearButton = tk.Button(master=buttonFrame, text="Clear Output", font=BUTTONFONT, height=buh, width=buw,
                            image=pixel, pady=0, padx=0, compound="center", command=clear)
    resetButton = tk.Button(master=buttonFrame, text="Reset Port", font=BUTTONFONT, height=buh, width=buw,
                            image=pixel, pady=0, padx=0, compound="center", command=reset)
    
        #generalRadioButton = tk.Radiobutton(master=generalRadioButtonFrame, text="General", bg=color, font=MEDFONT, variable=bu1, value=0, activebackground=color)

        #generalBox = tk.Checkbutton(master=generalBoxFrame, text="General", variable=bx1, bg=color, activebackground=color)
    autoscrollBox = tk.Checkbutton(master=boxFrame, text="Autoscroll", variable=bx1)
    timestampBox = tk.Checkbutton(master=boxFrame, text="Show Timestamp", variable=bx2)
    
        #generalScroll = tk.Scrollbar(master=generalScrollFrame, bg=color, variable=sc1)
    textScroll = tk.Scrollbar(master=scrollerFrame, command=outputCanvas.yview)
    outputCanvas.config(yscrollcommand=textScroll.set)
        
    #---------------Page Formatting

        #generalFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=110)
    body.pack(fill=tk.BOTH, expand=True)
    uiFrame.pack(side=tk.BOTTOM, fill=tk.X)
    boxFrame.pack(side=tk.LEFT, fill=tk.BOTH, padx=10)
    buttonFrame.pack(side=tk.RIGHT, fill=tk.BOTH, padx=10)
    printFrame.pack(side=tk.TOP, fill=tk.BOTH, expand=True)
    scrollerFrame.pack(side=tk.RIGHT, fill=tk.Y)
    textFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        #generalLabel.pack(fill=tk.BOTH, padx=10, side=tk.LEFT, expand=True, pady=15)
    outputCanvas.pack(fill=tk.BOTH, expand=True)

        #generalButton.pack(side=tk.LEFT, padx=20)
    clearButton.pack(side=tk.RIGHT, pady=2, padx=5)
    resetButton.pack(side=tk.LEFT, pady=2, padx=5)
    
        #generalRadioButton.grid(row=0,column=0,sticky="w")
    
        #generalBox.grid(row=0,column=0,sticky="w")
    autoscrollBox.pack(side=tk.LEFT, pady=2, padx=5)
    timestampBox.pack(side=tk.RIGHT, pady=2, padx=5)

        #generalScroll.pack()
    textScroll.pack(fill=tk.Y, expand=True)
    
    #---------------

    update()
    window.mainloop()
    
serial_monitor()
