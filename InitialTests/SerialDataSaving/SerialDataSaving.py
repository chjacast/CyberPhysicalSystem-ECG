import serial
from datetime import datetime

sensor = "Led"
serial_port = 'COM5'
baud_rate = 115200
path = "{0}_LOG_{1}.txt".format(str(datetime.now()).split(" ")[0], sensor)
ser = serial.Serial(serial_port, baud_rate)
with open(path, 'w') as f:
    while True:
        line = ser.readline()
        print(line)
        f.writelines([str(line), " t = %s \n " % (datetime.now())])