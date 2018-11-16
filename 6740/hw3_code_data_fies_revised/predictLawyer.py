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

labels = []
llik = []
numTrial = 3
from tempfile import TemporaryFile
for qq in range(numTrial):
  graph = np.load('graph_' + str(qq) + '.npy')
  graphMask = np.load('graphMask_' + str(qq) + '.npy')
  z = np.load('z_' + str(qq) + '.npy')
  W = np.load('W_' + str(qq) + '.npy')
  l, p = predict(graph, graphMask, z, W)
  labels.append(l)
  llik.append(p)

logProb = np.load('logProb_' + str(numTrial-1) + '.npy')
logProb = np.asarray(logProb).T
llik = np.asarray(llik).T
labels = np.asarray(labels).T

sBest, sProb = np.argmax(logProb[-1,:]), max(logProb[-1,:])
[sROC,sAlarm,sDetect] = areaROC(llik[:,sBest], labels[:,sBest], '-b')

