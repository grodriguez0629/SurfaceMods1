# serial library used for python
import pyserial
import time

#serial initialization
#serial port for communication
#baudrate
#timeout
SERIALPORT = 'ENTER'
BAUDRATE = 9600

board = serial.Serial(PORT, BAUDRATE)

writing = True
#start loop after initializing board
while writing:
    cmd = input("Enter command: ")
    if(cmd == False)
        writing = False
        return 0;
    board.write(cmd.encode)
    
