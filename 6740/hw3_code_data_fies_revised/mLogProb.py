import numpy as np
from dirichlet_logprob import dirichlet_logprob


def mLogProb(adj, z, pi, W, alpha):
    """
    mLogProb    Joint log-probability for configuration of stochastic block model
    :param adj: NxN adjacency matrix for observed graph
    :param z: 1xN vector of community assignments
    :param pi: Kx1 vector of community membership probabilities
    :param W: KxK matrix of community interaction probabilities
    :return: joint log-probability of all variables
    """
    N = adj.shape[0]
    K = len(pi)

    mask = (np.ones((N, N)) - np.identity(N)) > 0
    mask = np.multiply(mask, adj >= 0)
    logProb = dirichlet_logprob(np.dot(alpha, np.ones(K)), pi)

    for kk in range(K):
        for ll in range(K):
            logProb = logProb + dirichlet_logprob(
                np.array([1., 1.]),
                np.array([W[kk, ll], 1-W[kk, ll]]))
    logPi = np.log(pi)
    for ii in range(N):
        logProb = logProb + logPi[int(z[ii])]
    logW = np.log(W)
    logMW = np.log(1 - W)

    for ii in range(N):
        for jj in mask[ii, :].ravel().nonzero():
            src = z[ii]
            rcv = z[jj]
            logProb = logProb + np.dot(adj[ii, jj], logW[src, rcv]) + np.dot((1 - adj[ii, jj]), logMW[src, rcv])
    
    return logProb