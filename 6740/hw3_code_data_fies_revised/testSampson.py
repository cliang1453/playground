import os
import sys
import numpy as np 
from scipy.io import loadmat
from mGibbs import *
import matplotlib.pyplot as plt

numTrial = 5
numIter = 1000

# Load Sampson dataset
dirpath = os.path.split(os.path.abspath(__file__))[0]
sys.path.append(dirpath)
os.chdir(dirpath)
file = loadmat('sampson_network.mat')
graph = file['graph']
graph = graph.astype(np.int16)
monk_factions = np.ravel(file['monk_factions'])

K = 3

# Run Gibbs sampler, plot results
alphaSB = 1.0
logProb = []
randI = []
for qq in range(numTrial):
    z, pi, W, lp, ri = mGibbs(graph, K, alphaSB, numIter, monk_factions)
    logProb.append(lp)
    randI.append(ri)

logProb = np.asarray(logProb).T
randI = np.asarray(randI).T

plt.plot(logProb)
plt.xlabel('Iteration')
plt.ylabel('Log probability')
plt.show()

plt.plot(randI)
plt.xlabel('Iteration')
plt.ylabel('Rand index')
plt.show()
