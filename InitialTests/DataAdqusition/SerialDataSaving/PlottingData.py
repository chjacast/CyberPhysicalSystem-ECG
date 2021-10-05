from os import times
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
time=[]
ecg=[]

column_names=['date', 'time', 'ecg']
restEcg= pd.read_csv('S1A2M3F2021-09-14_LOG_ECG.txt', names=column_names,sep=' ')

plt.plot(restEcg.time,restEcg.ecg)


#fig = plt.figure()

#ax1 = fig.add_subplot(111)

#ax1.set_title("Plot title")    
#ax1.set_xlabel('x label')
#ax1.set_ylabel('y label')

#ax1.plot(time,ecg)

#leg = ax1.legend()

plt.show()