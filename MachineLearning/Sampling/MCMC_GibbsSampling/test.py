import os
import sys
from scipy.io import loadmat
from mGibbs import *
import matplotlib.pyplot as plt



numTrial = 5
numIter  = 1000

N = 30
K = 3
zT = np.zeros(N, dtype=int)
zT[10:20] = 1
zT[20:30] = 2
W = 0.1*np.ones(3) + (0.95-0.10)*np.eye(3)

adj = np.zeros((N,N))
for ii in range(N):
  for jj in range(N):
    if jj != ii: 
      edgePr = W[zT[ii],zT[jj]]
      adj[ii,jj] = multinomial_rnd(np.array([1-edgePr, edgePr]), 1)
      adj[ii,jj] = adj[ii,jj] - 1


# Run Gibbs sampler, plot results
alphaSB = 1.0
logProb = []
randI = []
for qq in range(numTrial):
    [z,pi,W,p,I] = mGibbs(adj, K, alphaSB, numIter, zT)
    logProb.append(p)
    randI.append(I)

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


