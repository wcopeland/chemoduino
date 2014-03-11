__author__ = 'Wilbert'


import struct
import os
import re
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



def CheckSyntax(text):
    if len(text) > 63:
        # All commands must be less than 64 bytes.
        return False

    if len([x for x in text if x == '-']) > 5:
        # Commands may contain a maximum of 4 arguements.
        return False

    argument_lists = {'-get':['-wpin', '-rpin', '-power', '-pfreq', '-pwid'],
                      '-set':['-wpin', '-rpin', '-power', '-pfreq', '-pwid'],
                      '-start':['-cont', '-auto', '-pulse'],
                      'stop':[''],
                      '-exec':[''],
                      '-help':[''],}
    command_list = [k for k in argument_lists]

    command_list = '(' + ')|('.join(command_list) + ')'
    if not re.match(command_list, text):
        return False

    command = re.match(command_list, text).group(0)
    if command == '-get':
        m = re.match('-get [0-5] (.*)', text)
        if m is None:
            return False
        elif m.group(1) not in argument_lists['-get']:
            return False

    elif command == '-set':
        m = re.match('-set [0-5] (.*) \d+\.*\d*(.*)', text)
        if m is None:
            return False
        elif m.group(1) not in argument_lists['-set']:
            return False
        elif m.group(2) != '':
            return False

    elif command == '-start':
        m = re.match('-start [0-5] (.*)', text)
        if m is None:
            return False
        elif m.group(1) not in argument_lists['-start']:
            return False

    elif command == '-stop':
        m = re.match('-stop [0-5](.*)', text)
        if m is None:
            return False
        elif m.group(1) not in argument_lists['-stop']:
            return False

    elif command == '-exec':
        m = re.match('-exec (\w+.txt)(.*)', text)
        if m is None:
            return False
        elif m.group(2) not in argument_lists['-exec']:
            return False
        try:
            file = open(m.group(1), 'r')
        except:
            return False
        # Check each line for validity
        command_queue = []
        for line in file:
            if not CheckSyntax(line):
                return False
            command_queue.append(line.replace('\n',''))
            return command_queue

    elif command == '-help':
        print('To start pumps...')
        print('-start <pump_id> <arg>')
        print('Arguments: -cont, -auto, -pulse\n')

        print('To stop pumps...')
        print('-stop <pump_id>\n')

        print('To set pump attribute...')
        print('-set <pump_id> <arg> <val>')
        print('Arguements: -wpin, -rpin, -pfreq, -pwid, -power\n')

        print('To get pump attribute...')
        print('-set <pump_id> <arg>')
        print('Arguements: -wpin, -rpin, -pfreq, -pwid, -power\n\n')

    return True

print('Available ports: {0}'.format(','.join([x for x in ListSerialPorts()])))
current_port = raw_input('Which port would you like to connect to? ')

while(current_port not in ListSerialPorts()):
    current_port = raw_input('Port "{0}" not found.\nWhich port would you like to connect to? '.format(current_port))


ser = serial.Serial(current_port, 9600)
ser.setDTR()
ser.setRTS()

command = raw_input('Enter command: ')
while command != 'exit':

    valid = CheckSyntax(command)
    if valid is False:
        print('Invalid command.')
        command = raw_input('Enter command: ')
        continue
    elif type(valid) is list:
        for command in valid:
            ser.write(command)
            print(ser.readline())
        command = raw_input('Enter command: ')
    else:
        # Send command
        ser.write(command)
        print(ser.readline())
        command = raw_input('Enter command: ')

ser.close()
