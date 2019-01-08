from scipy.special import gammaln
import math
import numpy as np 
def dirichlet_logprob(alpha, x):
    """ Log probability for Dirichlet distribution """
    # alpha = K-dim. vector of Dirichlet parameters
    logProb = gammaln(np.sum(alpha)) - np.sum(gammaln(alpha)) 
    logProb = logProb + np.sum(np.log(x) * (alpha-1))
    return logProb
