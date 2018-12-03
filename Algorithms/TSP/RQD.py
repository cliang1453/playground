import numpy as np
import sys
import glob
import math
import matplotlib.pyplot as plt

t = [0.001, 0.005, 0.01, 0.5, 5]
q = list(np.array([[2.16043,2.22898,2.23999,2.24622,2.24967,2.28454,2.29342,2.31093,2.38663,2.39273],
[1.45452,1.46089,1.47216,1.48714,1.5008,1.56112,1.56881,1.57041,1.57173,1.57332], 
[1.23535,1.31901,1.33744,1.33771,1.34752,1.36144,1.37497,1.41209,1.46553,1.50398],
[1.11283,1.15646,1.17754,1.19186,1.20154,1.2319,1.24914,1.25975,1.31583,1.38186], 
[1.10965,1.12198,1.15646,1.18218,1.19159,1.22302,1.2319,1.24874,1.24914,1.28825]])-1) 

x = list(np.arange(0, 1, 0.1)+0.1)
for i in range(4):
	plt.plot(q[i], x, label="Time =" + str(t[i]-1))
plt.xlabel('Relative quality(s)')
plt.ylabel('P(solved)')
plt.legend()
plt.title('SQD for Simulated Annealing (Berlin)')
plt.show()