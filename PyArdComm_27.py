__author__ = 'Wilbert'


import struct
import os
import serial
from serial.tools import list_ports



def ListSerialPorts():

    # Windows
    if os.name == 'nt':
        # Scan for available ports.
        available_ports = []
        for i in range(256):
            try:
                s = serial.Serial(i)
                available_ports.append('COM'+str(i + 1))
                s.close()
            except serial.SerialException:
                pass
        return available_ports
    else:
        # Mac / Linux
        return [port[0] for port in list_ports.comports()]


def packAsInt(value):
    """Packs values as a 4-byte integer. """
    return struct.pack('i', value)    #should check bounds

def packAsFloat(value):
    """ Packs value as a 4-byte float. """
    return struct.pack('f', value)




print('Available ports: {0}'.format(','.join([x for x in ListSerialPorts()])))
current_port = raw_input('Which port would you like to connect to? ')

while(current_port not in ListSerialPorts()):
    current_port = raw_input('Port "{0}" not found.\nWhich port would you like to connect to? '.format(current_port))



#ser = serial.Serial(current_port, 9600)
ser = serial.Serial('COM11', 9600)
ser.setDTR()
ser.setRTS()

command = raw_input('Enter command: ')
while command != 'exit':

    if len(command) > 63:
        print('Commands must be less than 64 bytes!')
        command = raw_input('Enter command: ')
        continue

    if len([x for x in command if x == '-']) > 5:
        print('Commands must not pass more than 4 arguements!')
        command = raw_input('Enter command: ')
        continue

    ser.write(command)
    print(ser.readline())
    command = raw_input('Enter command: ')

ser.close()