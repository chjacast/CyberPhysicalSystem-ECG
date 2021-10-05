import serial
import time
from datetime import datetime

sensor = "ECG"
serial_port = 'COM5' #Se configura el puerto por donde se encontrara el micro
baud_rate = 9600 #Se configura el baud rate depediendo de como este configurado el micro
path = "{0}_LOG_{1}.csv".format(str(datetime.now()).split(" ")[0], sensor)
ser = serial.Serial(serial_port, baud_rate)

#Tiempo actual
Time_init=float(str(datetime.now()).split(" ")[1].split(":")[2])
Time_act=0
print(Time_act)
time.sleep(4)
with open(path, 'w+') as f:
    while (Time_act - Time_init) < 10: #Se va a recoger los datos durante n segundos
        line = ser.readline()
        print(line)
        Time_act=float(str(datetime.now()).split(" ")[1].split(":")[2])
        f.writelines([str(datetime.now())+" ", str(line)[2:-5]+'\n'])
f.close()
