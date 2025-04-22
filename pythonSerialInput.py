# serial library used for python
# tkinter is a gui library
import sys
import serial
from tkinter import *
from tkinter import ttk

#serial initialization
#serial port for communication
#baudrate
SERIALPORT = 'COM6'
BAUDRATE = 9600
board = serial.Serial(port=SERIALPORT, baudrate=BAUDRATE)

#sends rover movement to the board
def send_movement(steps, dir):
    print("moving rover " + steps.get() + " steps " + dir.get())
    cmd =  dir.get() + steps.get() + "\n"
    print(cmd)
    board.write(cmd.encode())

#sends shoulder movement to the board
def send_shoulder(deg, side, dir):
    print("moving " + side.get() + " shoulder " + dir.get() + " " + deg.get() + " degrees")
    cmd = side.get() + dir.get() + deg.get()
    print(cmd)
    board.write(cmd.encode())
    
#sends auger movemnt
def send_auger(spd_prof, dir):
    print("starting auger at " + spd_prof.get() + " in the " + dir.get() + " direction")
    cmd = spd_prof.get() + " " + dir.get()
    print(cmd)
    board.write(cmd.encode())

#exits program
def quit():
    print("Exiting...")
    sys.exit()

#limits character count of steps to 3
def limit_char_count(text, max_length):
    if len(text.get()) > max_length: 
        text.set(text.get()[:max_length])

#enables or disables buttons based on stringvar states
def enable_btn(btn, arg1, arg2):
    if(arg1.get() != '' and arg2.get() != ''):
        btn.state(['!disabled'])
    elif(arg1.get() == '' or arg2.get() == ''):
        btn.state(['disabled'])

def enable_btn2(btn, arg1, arg2, arg3):
    if(arg1.get() != '' and arg2.get() != '' and arg3.get() != ''):
        btn.state(['!disabled'])
    elif(arg1.get() == '' or arg2.get() == '' or arg3.get() == ''):
        btn.state(['disabled'])

def main():
    root = Tk()
    root.title("RASSOR movement interface")
    frame = ttk.Frame(root, padding=10)
    frame.grid()
    
    #title label
    ttk.Label(frame, text="COMMANDS").grid(column=0, row=0)
    
    #rover movement handling UI
    step_entry = StringVar()
    step_entry.trace_add("write", lambda *args: limit_char_count(step_entry, 3))
    step_entry.trace_add("write", lambda *args: enable_btn(move_btn, step_entry, move_flrb))
    move_flrb = StringVar()
    
    move_btn = ttk.Button(frame, text="MOVE ROVER",
                          command=lambda *args: send_movement(step_entry, move_flrb), state='disabled')
    move_btn.grid(column=0, row=1)
    
    ttk.Label(frame, text="Steps to Move").grid(column=1, row=1)
    ttk.Entry(frame, 
              width=5, 
              textvariable=step_entry).grid(column=2, row=1)
    ttk.Radiobutton(frame, text="↑",
                    variable=move_flrb,
                    value='F',
                    command=lambda *args: enable_btn(move_btn, step_entry, move_flrb)).grid(column=2, row=2)
    ttk.Radiobutton(frame, text="←",
                    variable=move_flrb,
                    value='L',
                    command=lambda *args: enable_btn(move_btn, step_entry, move_flrb)).grid(column=1, row=3)
    ttk.Radiobutton(frame, text="→",
                    variable=move_flrb,
                    value='R',
                    command=lambda *args: enable_btn(move_btn, step_entry, move_flrb)).grid(column=3, row=3)
    ttk.Radiobutton(frame, text="↓",
                    variable=move_flrb,
                    value='B',
                    command=lambda *args: enable_btn(move_btn, step_entry, move_flrb)).grid(column=2, row=4)

    #shoulder movement handling UI
    deg_entry = StringVar()
    deg_entry.trace_add("write", lambda *args: limit_char_count(step_entry, 2))
    deg_entry.trace_add("write", lambda *args: enable_btn2(shoulder_btn, deg_entry, shoulder_lr, shoulder_ud))
    shoulder_lr = StringVar()
    shoulder_ud = StringVar()
    
    shoulder_btn = ttk.Button(frame, text="MOVE SHOULDER", 
                              command=lambda *args: send_shoulder(deg_entry, shoulder_lr, shoulder_ud), state='disabled')
    shoulder_btn.grid(column=0, row=5)
    ttk.Label(frame, text="Degrees to Rotate").grid(column=1, row=5)
    ttk.Entry(frame, 
              width=5, 
              textvariable=deg_entry).grid(column=2, row=5)
    ttk.Label(frame, text="Shoulder to Rotate").grid(column=1, row=6)
    ttk.Radiobutton(frame, text="Left", 
                    variable=shoulder_lr, 
                    value='L', 
                    command=lambda *args: enable_btn2(shoulder_btn, deg_entry, shoulder_lr, shoulder_ud)).grid(column=1, row=7)
    ttk.Radiobutton(frame, text="Right", 
                    variable=shoulder_lr, 
                    value='R', 
                    command=lambda *args: enable_btn2(shoulder_btn, deg_entry, shoulder_lr, shoulder_ud)).grid(column=2, row=7)
    ttk.Label(frame, text="Direction to Rotate").grid(column=1, row=8)
    ttk.Radiobutton(frame, text="Up", 
                    variable=shoulder_ud, 
                    value='U', 
                    command=lambda *args: enable_btn2(shoulder_btn, deg_entry, shoulder_lr, shoulder_ud)).grid(column=1, row=9)
    ttk.Radiobutton(frame, text="Down", 
                    variable=shoulder_ud, 
                    value='D', 
                    command=lambda *args: enable_btn2(shoulder_btn, deg_entry, shoulder_lr, shoulder_ud)).grid(column=2, row=9)
    
    #auger handling UI
    spd_prof = StringVar()
    direction = StringVar()
    
    auger_btn = ttk.Button(frame, text="START AUGER", 
                              command=lambda *args: send_auger(spd_prof, direction), state='disabled')
    auger_btn.grid(column=0, row=10)
    ttk.Label(frame, text="Speed Profile").grid(column=1, row=10)
    ttk.Combobox(frame, textvariable=spd_prof, values=['OFF', 'LOW', 'HIGH']).grid(column=1, row=11)
    ttk.Label(frame, text="Direction").grid(column=1, row=12)
    ttk.Radiobutton(frame, text="CW", 
                    variable=direction, 
                    value='CW', 
                    command=lambda *args: enable_btn(auger_btn, spd_prof, direction)).grid(column=1, row=14)
    ttk.Radiobutton(frame, text="CCW", 
                    variable=direction, 
                    value='CCW', 
                    command=lambda *args: enable_btn(auger_btn, spd_prof, direction)).grid(column=2, row=14)
    
    #quit program
    ttk.Button(frame, text="QUIT", command=quit).grid(column=0, row=99)
    
    root.mainloop()
    
    #to write to board, needs to be encoded to bytes
    #decode the encoded bytes on the board
    # print(cmd)
    # board.write(cmd.encode())

#start loop after initializing board
if __name__ == "__main__": 
    main()
    
