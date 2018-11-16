import numpy as np
import multinomial_rnd
from dirichlet_rnd import *
from mLogProb import *
from valid_RandIndex import valid_RandIndex
from multinomial_rnd import *
from gamm_rnd import *

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
        a = np.repeat(alpha, K)
        for k in range(K):
            c = np.count_nonzero(z == k)
            a[k] += c
        pi = dirichlet_rnd(a, 1)
        
        # TODO: sample community interaction parameters W
        for kk in range(K):
            for ll in range(K):
                i_cand = np.argwhere(z == kk)
                j_cand = np.argwhere(z == ll)
                sum_ij = 0
                sum_yij = 0

                for i in i_cand:
                    for j in j_cand:
                        if not mask[i, j]:
                            continue
                        sum_ij += 1
                        sum_yij += adj[i,j]
                W[kk][ll] = np.random.beta(1 + sum_yij, 1 + sum_ij - sum_yij)
        
        
        # TODO: sample community assignments z in random order
        for ii in np.random.permutation(N):
            # construct a categorical distribution
            p_z = np.zeros(K)
            for k in range(K):
                prior = pi[k]
                likelihood = 1
                for j in range(N):
                    if not mask[ii, j]:
                        continue
                    likelihood *= np.power(W[k][z[j]], adj[ii][j]) * np.power(1-W[k][z[j]], 1-adj[ii][j]) \
                               * np.power(W[z[j]][k], adj[j][ii]) * np.power(1-W[z[j]][k], 1-adj[j][ii])
                p_z[k] = prior * likelihood

            # sampling from categorical distribution using inverse sampling
            p_z = p_z/np.sum(p_z)
            cdf_z = {}
            cdf_z[0] = p_z[0]
            for k in range(1, K):
                cdf_z[k] = p_z[k] + cdf_z[k-1]
            a = np.random.uniform(0, 1)
            for k, v in cdf_z.items():
                if v > a:
                    z[ii] = k
                    break

        logProb[tt] = mLogProb(adj, z, pi, W, alpha)
        AR, RI, MI, HI = valid_RandIndex(z, zTrue)
        randI[tt] = RI
        if tt % round(numIter / 10) == 0:
            print('.')
    return [z, pi, W, logProb, randI]
