import os
import sys
import copy
import random 
from scipy.io import loadmat
from mGibbs import *
from predict import *
import matplotlib.pyplot as plt
from areaROC import *
import operator

numTrial = 3
numIter = 1000

dirpath = os.path.split(os.path.abspath(__file__))[0]
sys.path.append(dirpath)
os.chdir(dirpath)


file = loadmat('lawyers_advice_network.mat')
K = 6
graph = file['graph']


# Mask out test data
N = len(graph)
graph = graph.astype(np.int16)
graphMask = copy.copy(graph)

for ii in range(N):
  for jj in range(N):
    if ii != jj:
      testFlag = (random.uniform(0,1) > 0.5)
      if testFlag:
        graphMask[ii,jj] = -1

# Run Gibbs sampler
alphaSB = 1.0
logProb = []
labels = []
llik = []
for qq in range(numTrial):
  z,pi,W,lp,_ = mGibbs(graphMask, K, alphaSB, numIter)
  logProb.append(lp)
  l, p = predict(graph, graphMask, z, W)
  labels.append(l)
  llik.append(p)

logProb = np.asarray(logProb).T
llik = np.asarray(llik).T
labels = np.asarray(labels).T

# Plot results
# plt.plot(logProb)
plt.xlabel('Iteration')
plt.ylabel('Log probability')


sBest, sProb = np.argmax(logProb[-1,:]), max(logProb[-1,:])
[sROC,sAlarm,sDetect] = areaROC(llik[:,sBest], labels[:,sBest], '-b')
