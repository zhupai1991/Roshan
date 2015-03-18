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
f.write(str(num_node)+'\n')

for i in range(0, num_node):
    f.write(str(x[i]) + ' ' + str(y[i]) + ' ' + str(z[i]) +  '\n')

f.write(str(time_step) + '\n')

for t in range(0, time_step+1):
    f.write(str(time[t-1]) + '\n')
    for ip in range(0, num_node):
        f.write(str(qc[ip]+(t-1)*0.01) + ' ' + str(ts[ip]+(t-1)*10) + '\n')
    
f.close()
