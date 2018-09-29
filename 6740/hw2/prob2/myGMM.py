#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Sep 15 19:11:07 2018

@author: yujia
"""
import numpy as np
import random

def myGMM(X, K, maxIter):

    '''
    This is a function that performs GMM clustering
    The input is X: N*d, the input data
                 K: integer, number of clusters
                 maxIter: integer, number of iterations
             
    The output is C: K*d the center of clusters
                  I: N*1 the label of data
                  Loss: [maxIter] likelihood in each
                  step
    '''
    
    # number of vectors in X
    [N, d] = size(X)
    
    # construct indicator matrix (each entry corresponds to the cluster of each point in X)
    I = np.zeros(N, 1)
    
    # construct centers matrix
    C = np.zeros(K, d)
    
    # the list to record error
    Loss = []

    #####################################################################
    # TODO: Implement the EM method for Gaussian mixture model          #
    #####################################################################
    mu = X[np.random.choice(N, K, False), :]
    Sigma= [np.eye(d)] * K
    w = [1./K] * K
    #####################################################################
    #                      END OF YOUR CODE                             #
    #####################################################################
    return C, I, Loss