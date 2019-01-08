import sys
import numpy as np
import matplotlib.pyplot as plt
import random 

seed = 42
def areaROC(confidence, testClass, plotStyle=''):
  #     areaROC     Compute and plot ROC curve and corresponding area
  # 
  #     [aROC, alarmRate, detectRate] = areaROC(confidence, testClass, plotStyle)
  # 
  #     Compute ROC curve, where
  #     confidence = vector of scores for each test example, where higher scores
  #                  indicate greater confidence in target presence
  #     testClass  = vector giving ground truth for each test example, where
  #                  1 indicates target presence and 0 target absence
  #     plotStyle  = text string giving style for plotted ROC curve, or
  #                  empty to not create plot (DEFAULT)
  #     aROC       = area under ROC curve
  #     alarmRate  = false alarm rates, uniformly sampled on [0,1]
  #     detectRate = corresponding detection rates computed from given confidence


  # % number of false alarm rates at which to evaluate curve
  numpoints = 500
  
  # % perturb confidence to avoid "staircase" effect
  random.seed(seed)
  confidence = confidence + np.power(np.random.rand(len(confidence))*10, -10)
  
  # % indices of negative and positive test cases
  ndxAbs = np.where(np.ravel(testClass.T) <= 0)   #absent
  ndxPres  = np.where(np.ravel(testClass.T) >= 1) # present
  confAbs  = np.sort(confidence[ndxAbs])
  confPres = np.sort(confidence[ndxPres])

# % compute ROC curve
 
  confAbsResamp = confAbs[np.fix(np.linspace(1, len(confAbs), numpoints, False)).astype(np.int16)]

  alarmRate  = np.zeros(numpoints)
  detectRate = np.zeros(numpoints)
  for ii in range(numpoints):
     detectRate[ii] = sum(confPres>= confAbsResamp[ii]) / len(ndxPres[0])
     alarmRate[ii] = sum(confAbs >= confAbsResamp[ii]) / len(ndxAbs[0])

  # % compute area under ROC curve
  aROC = abs(sum((alarmRate[1:]-alarmRate[:-1])*(detectRate[1:]+detectRate[:-1])/2))
  # % if desired, plot ROC curve
  if plotStyle != '':
    plt.plot(alarmRate, detectRate, plotStyle)
    locs, labels = plt.xticks()
    plt.xticks(locs, [0,1,0,1])
    plt.ylabel('Detection Rate')
    plt.xlabel('False Alarm Rate')
    plt.show()

  return [aROC, alarmRate, detectRate]
