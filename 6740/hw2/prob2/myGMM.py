#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Sep 15 19:11:07 2018

@author: yujia
"""
import numpy as np
import random

def P(mu, Sigma, X, K, N, d):
    '''
    input: mu: K*d
           Sigma: K*d*d
           X: N*d 
    output: N*K
    '''
    X = np.expand_dims(X, axis=2) # N*d*1
    mu = np.expand_dims(mu, axis=2) # K*d*1
    Normal = np.zeros([N, K])

    for k in range(K):

        pref_k = np.power(np.linalg.det(Sigma[k, :, :]), -1/2.0) * np.power(2.0*np.pi, -d/2.0)
        Sigma_inv_k = np.linalg.inv(Sigma[k, :, :])
        mu_k = mu[k, :, :]

        for n in range(N):
            Normal[n, k] = pref_k * np.exp(-1/2.0 * np.dot(np.dot((X[n, :, :] - mu_k).T, Sigma_inv_k), X[n, :, :] - mu_k))
    
    return Normal


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
    N, d = X.shape
    
    # construct indicator matrix (each entry corresponds to the cluster of each point in X)
    I = np.zeros([N, 1])
    
    # construct centers matrix
    C = np.zeros([K, d]) # Did TA test their code before releasing???
    
    # the list to record error
    Loss = []

    #####################################################################
    # TODO: Implement the EM method for Gaussian mixture model          #
    #####################################################################
    
    # mu: K*d
    mu = X[np.random.choice(N, K), :]
    # Sigma: K*d*d
    Sigma = np.stack([np.eye(d) for _ in range(K)], axis=0)
    # pi: K
    pi = np.full(K, 1.0/K)
    # gamma
    gamma = np.zeros([N, K])

    while len(Loss) < maxIter:
        
        # Normal: N*K
        Normal = P(mu, Sigma, X, K, N, d)
        gamma = pi * Normal

        # log likelihood
        Loss.append(np.sum(np.log(np.sum(gamma, axis=1))))

        # normalized_gamma: N*K
        gamma = gamma / np.sum(gamma, axis=1, keepdims=True)
        N_gamma = np.sum(gamma, axis=0)
        # update mu: K*d & update Sigma: K*d*d
        for k in range(K):
            
            mu[k, :] = 1.0 / N_gamma[k] * np.sum(np.expand_dims(gamma[:, k], axis=1) * X, axis=0)
            gamma_eye = np.eye(N) * gamma[:, k]
            Sigma[k, :, :] = 1.0 / N_gamma[k] * np.dot(np.dot((X-mu[k, :]).T, gamma_eye), X-mu[k, :]) 
            pi[k] = N_gamma[k]/N

    C = mu
    I = np.argmax(gamma, axis=1)
    #####################################################################
    #                      END OF YOUR CODE                             #
    #####################################################################
    return C, I, Loss