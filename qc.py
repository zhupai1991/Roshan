#!/usr/bin/env python
import numpy as np
num_node = 11
time_step = 2
time = [0, 1]
n = np.linspace(0, 1, num_node)
x = n*0
y = n
z = n*0
qc = 0.1*n
ts = 300*n
f = open('qc.dat', 'w')
f.write(str(num_node)+ ' ' +str(time_step) +'\n')


for t in range(0, time_step):
    for i in range(0, num_node):
        f.write(str(x[i]) + ' ' + str(y[i]) + ' ' + str(z[i]) +' ' + str(qc[i]) +' '+str(ts[i]) +' '+ str(0) +' ' + str(0) + ' ' + str(time[t]) +'\n')
    
f.close()
