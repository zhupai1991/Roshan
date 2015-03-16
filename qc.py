#!/usr/bin/env python
import numpy as np
num_node = 10
n = np.linspace(0, 1, num_node)
x = n*0
y = n
z = n*0
qc = n
f = open('qc.dat', 'w')
f.write(str(num_node)+'\n')

for i in range(0, num_node):
    f.write(str(x[i]) + ' ' + str(y[i]) + ' ' + str(z[i]) + ' ' + str(qc[i]) + '\n')
    
f.close()
