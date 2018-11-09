import random
import numpy as np

def multinomial_rnd(p,n):
#    multinomial_rnd     Sample from multinomial distribution
#
#    ddraw = multinomial_rnd(p, N)
#
#    Draws N independent samples from a multinomial distribution, where
#    p     = K-dim. vector of multinomial weights (possibly unnormalized)
#    ddraw = N-dim. vector containing multinomial samples in {1,...,K}

    ps = np.cumsum(p)

    if n == 1:
        s = sum(ps[-1]*random.random() > ps) + 1
    else:
        s = ps[-1]*np.random.rand(n)
        for i in range (n):
            s[i] = sum(s[i] > ps) + 1
    return s
