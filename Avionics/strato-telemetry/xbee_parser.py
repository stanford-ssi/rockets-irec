from __future__ import print_function
import sys
import time
import glob
import serial
import struct
import numpy
import csv
import matplotlib.pyplot as plat

now = time.strftime("%Y%m%d-%H%M")
FNAME = "IREC_STRATO-TELEMETRY_" + now + ".log"

def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

print("test")
#print(serial_ports())
xbee = serial.Serial('COM10',9600)
f = open(FNAME, "a")
print(f)
while(1):
    #try:
        byte = xbee.read(1)
        print(byte,end="")
        s = str(byte)
        #print(s)
        f.write(s)
        f.close()
        f = open(FNAME, "a")
