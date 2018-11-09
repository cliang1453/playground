import numpy as np

def dirichlet_rnd(alpha, numsamps):
    """Sample from Dirichlet distribution"""
    # ddraw = dirichlet_rnd(alpha, N)
    # Draws N independent samples from a Dirichlet distribution, where
    # alpha = K-dim. vector of Dirichlet parameters
    # ddraw = KxN matrix where each column is a sample
    kdim = len(alpha)
    al = np.zeros([kdim, numsamps])
    for k in range(kdim):
        al[k, :] = np.random.gamma(1, numsamps, (int(alpha[k]),1))
    ddraw = al/np.tile(np.sum(al,axis=0), (kdim, 1))
    return ddraw
