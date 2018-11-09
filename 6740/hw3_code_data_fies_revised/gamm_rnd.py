import numpy as np
import math
import random
def gamm_rnd(nrow,ncol,m,k):
# PURPOSE: a matrix of random draws from the gamma distribution
#---------------------------------------------------
# USAGE: r = gamm_rnd(nrow,ncol,m,k)
# where: nrow,ncol = the size of the matrix drawn 
#        m = a parameter such that the mean of the gamma = m/k
#        k = a parameter such that the variance of the gamma = m/(k^2)
#        note: m=r/2, k=2 equals chisq r random deviate 
#---------------------------------------------------
# RETURNS:
#        r = an nrow x ncol matrix of random numbers from the gamma distribution      
# --------------------------------------------------
# SEE ALSO: gamm_inv, gamm_pdf, gamm_cdf
#---------------------------------------------------
# NOTE: written by: Michael Gordy, 15 Sept 1993
#                   mbgordy@athena.mit.edu
#---------------------------------------------------
# REFERENCES: Luc Devroye, Non-Uniform Random Variate Generation, 
#            New York: Springer Verlag, 1986, ch 9.3-6.

    gb = np.ones(nrow,ncol)*0
    if m <= 1:
        #Use RGS algorithm by Best, p. 426
        c=1/m
        t=0.07+0.75*math.sqrt(1-m)
        b=1+math.exp(-t)*m/t
        for i1 in range(1,nrow+1):
            for i2 in range(1,ncol+1):
                accept = 0
                while (accept == 0):
                    u = random.uniform(0,1)
                    w = random.uniform(0,1)
                    v = b*u
                    if v <= 1:
                        x = t*(v**c)
                        accept = (w<=(2-x)/(2+x)) or (w<=math.exp(-x))
                    else:
                        x = -math.log(c*t*(b-v))
                        y = x/t
                        accept = ((w*(m+y-m*y))<=1) or (w<=(y**(m-1)))
                gb[i1,i2] = x
    else:
        # Use Best's rejection algorithm XG, p. 410
        b=m-1
        c=3*m-0.75
        for i1 in range(1,nrow+1):
            for i2 in range(1,ncol+1):
               accept=0
               while (accept==0):
                  u=random.uniform(0,1)
                  v=random.uniform(0,1)
                  w=u*(1-u)
                  y=math.sqrt(c/w)*(u-0.5)
                  x=b+y
                  if x >= 0:
                     z=64*(w**3)*v*v
                     accept=(z<=(1-2*y*y/x)) or (math.log(z)<=(2*(b*math.log(x/b)-y)))
               gb[i1,i2] = x
    gb = gb/k
    return gb


                        
