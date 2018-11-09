import numpy as np
import multinomial_rnd
from mLogProb import *
from valid_RandIndex import valid_RandIndex
from multinomial_rnd import *

def mGibbs(adj, K, alpha=None, numIter=None, zTrue=None, zInit=None):
    """
    Gibbs sampler
    :param adj: NxN adjacency matrix for observed graph,
                where negative entries indicate missing observations
    :param K: number of communities
    :param alpha: Dirichlet concentration parameter
    :param numIter: number of Gibbs sampling iterations
    :param zInit: 1xN vector of initial community assignments
    :return: z: 1xN vector of final community assignments
            pi: Kx1 vector of community membership probabilities
            W: KxK matrix of community interaction probabilities
    """
    N = adj.shape[0]
    mask = (np.ones((N, N)) - np.identity(N)) > 0
    mask = np.multiply(mask, (adj >= 0))
    if alpha is None:
        alpha = 1.0
    if numIter is None:
        numIter = 1
    if zTrue is None:
        zTrue = np.ones(N, dtype=int)
    if zInit is None:
        z = multinomial_rnd(np.ones(K), N) 
        z = z.astype(int) - 1 
    else:
        z = zInit

    pi = np.random.uniform(0,1, K)
    pi = pi/np.sum(pi)
    W = np.random.uniform(0, 1, (K,K))
    logProb = np.zeros(numIter)
    randI = np.zeros(numIter)
    print('SB Gibbs: ')
    for tt in range(numIter):
        # TODO: sample mixture probabilities pi, pi = ?
        pi = pi
        # TODO: sample community interaction parameters W
        for kk in range(K):
            for ll in range(K):
                # W[kk,ll] = ?
                pass
        # TODO: sample community assignments z in random order
        for ii in np.random.permutation(N):
            # z[ii] = ?
            
        logProb[tt] = mLogProb(adj, z, pi, W, alpha)
        AR, RI, MI, HI = valid_RandIndex(z, zTrue)
        randI[tt] = RI
        if tt % round(numIter / 10) == 0:
            print('.')
    return [z, pi, W, logProb, randI]
