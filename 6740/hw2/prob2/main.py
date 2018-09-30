#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Sep 15 19:04:16 2018

@author: yujia
"""
import scipy
from scipy.io import loadmat

import matplotlib.pyplot as plt

import matplotlib 
fontsize = 20
matplotlib.rc('xtick', labelsize=fontsize) 
matplotlib.rc('ytick', labelsize=fontsize) 

from myGMM import *

def plot(X, C, K):   
    #####################################################################
    # TODO: Implement the plot function                                 #
    #####################################################################
    plt.scatter(X[:,0], X[:,1], c=C)
    plt.title('Visualization of K = '+str(K), fontsize=fontsize)
    plt.show()
    #####################################################################
    #                      END OF YOUR CODE                             #
    #####################################################################
    
    
def plot_losses(Losses):   
    #####################################################################
    # TODO: Implement the plot function                                 #
    #####################################################################
    plt.plot(Losses[0])
    plt.plot(Losses[1])
    plt.plot(Losses[2])
    plt.title('Plot of losses', fontsize=fontsize)
    plt.show()
    #####################################################################
    #                      END OF YOUR CODE                             #
    #####################################################################

if __name__ == "__main__":
    data = scipy.io.loadmat('data/Q2.mat')['X']
    
    #Set parameters
    max_iter = 100 # choose one that is suitable
    
    Losses = []
    for K in [2, 3, 4]: #You should do K=2, 3, 4
        # Do clustering
        C, I, Loss = myGMM(data, K, max_iter)
        
        # Plot your result
        plot(data, I, K)
        Losses.append(Loss)
        
    #plot the losses together
    plot_losses(Losses)
