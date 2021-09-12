from os import times
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
time=[]
ecg=[]
#with open('2021-09-12_LOG_ECG.txt') as f:
#    lines = f.readlines()
#    for line in lines:
#        line=line.split()
#        time.append(line[1].split(':')[2])
#        ecg.append(line[2])
column_names=['date', 'time', 'ecg']
restEcg= pd.read_csv('Acostado_2021-09-12_LOG_ECG.txt', names=column_names,sep=' ')

plt.plot(restEcg.time,restEcg.ecg)


#fig = plt.figure()

#ax1 = fig.add_subplot(111)

#ax1.set_title("Plot title")    
#ax1.set_xlabel('x label')
#ax1.set_ylabel('y label')

#ax1.plot(time,ecg)

#leg = ax1.legend()

plt.show()