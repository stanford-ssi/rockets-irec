import sys
import glob
import serial
import struct
import numpy


FILE_PATH = './fakedata/fake1.csv'


FSTART      = b'\xaa'
FESENSE     = b'\x01'
FISOSENSE   = b'\x02'
FBNO        = b'\x03'
FMMA        = b'\x04'
FBMP        = b'\x05' 

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


if __name__ == '__main__':
	#reading CSV file into memory, sorting into arrays for each sensr
	all_data = numpy.genfromtxt(FILE_PATH, delimiter=',')
	bno_data = all_data[all_data[:,0] == 3.0, :][:,1:3]



	print(bno_data)
	print(serial_ports())
	teensy = serial.Serial('COM6')
	#wait for teensy configured for SITL testing
	while(1):
		read = teensy.read(1)
		print(read)
		if read == FSTART:
			print('SALT found ready for SITL testing')
			teensy.write(FSTART)
			break
	while(1):
		request = teensy.read(5)
		sensor = request[0]
		time = struct.unpack('f',request[1:5])
		if sensor == FBMP:
			print('SALT Requesting BMP data')
			ind = numpy.searchsorted(bno_data[:,0],time)
			if abs(time - bno_data[ind,0]) > abs(time - bno_data[ind-1,0]):
				ind += 1
			fetch = bno_data[ind,1:2]



        
        

    
    
    
    
    
    
    




