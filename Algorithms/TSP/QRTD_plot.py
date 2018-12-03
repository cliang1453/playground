import numpy as np
import sys
import glob
import math
import matplotlib.pyplot as plt

q =[1.10965,1.24529, 1.38093,1.51657 ]
y = [[0.222,0.251,0.299,0.582,1.248,1.26,1.518,2.421, 2.549, 2.569],
[0.009,0.022,0.037,0.042,0.114,0.157,0.299,0.844,1.518, 2.569],
[0.005,0.006,0.006,0.006,0.008,0.008,0.009,0.011,0.015,0.749], 
[0.003,0.003,0.003,0.004,0.004,0.005,0.006,0.006,0.007,0.008]] 

x = list(np.arange(0, 1, 0.1)+0.1)

for i in range(4):
	plt.plot(y[i], x, label='q=' + str(q[i]-1))
plt.xlabel('Runtime CPU Seconds(s)')
plt.ylabel('P(solved)')
plt.legend()
plt.title('QRTD for Simulated Annealing (Berlin)')
plt.show()