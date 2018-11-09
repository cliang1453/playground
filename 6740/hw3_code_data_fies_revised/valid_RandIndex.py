import numpy as np
from scipy.misc import comb
import sys

def valid_RandIndex(c1,c2):
    # %RANDINDEX - calculates Rand Indices to compare two partitions
    # % ARI=RANDINDEX(c1,c2), where c1,c2 are vectors listing the
    # % class membership, returns the "Hubert & Arabie adjusted Rand index".
    # % [AR,RI,MI,HI]=RANDINDEX(c1,c2) returns the adjusted Rand index,
    # % the unadjusted Rand index, "Mirkin's" index and "Hubert's" index.
    # % See L. Hubert and P. Arabie (1985) "Comparing Partitions" Journal of
    # % Classification 2:193-218
    # %(C) David Corney (2000)   		D.Corney@cs.ucl.ac.uk

    if c1.ndim > 1 | c2.ndim > 1:
        raise Exception('RandIndex: vector size not met')

    C=Contingency(c1,c2)	#%form contingency matrix
    
    n=np.sum(C)
    nis=np.sum(np.power(np.sum(C,axis=1), 2))		#%sum of squares of sums of rows
    njs=np.sum(np.power(np.sum(C,axis=0), 2))		#%sum of squares of sums of columns

    t1=comb(n,2)		#%total number of pairs of entities
    t2=np.sum(np.sum(np.power(C,2))) #%sum over rows & columnns of nij^2
    t3=.5*(nis+njs)

    # %Expected index (for adjustment)
    nc=(n*(n**2+1)-(n+1)*nis-(n+1)*njs+2*(nis*njs)/n)/(2*(n-1))

    A=t1+t2-t3		#%no. agreements
    D= -t2+t3		#%no. disagreements

    if t1==nc:
        AR=0			#%avoid division by zero; if k=1, define Rand = 0
    else:
        AR=(A-nc)/(t1-nc)		#%adjusted Rand - Hubert & Arabie 1985

    RI=A/t1		#%Rand 1971		%Probability of agreement
    MI=D/t1		#%Mirkin 1970	%p(disagreement)
    HI=(A-D)/t1	#%Hubert 1977	%p(agree)-p(disagree)
    return [AR, RI, MI, HI]

def Contingency(Mem1,Mem2):
    if Mem1.ndim > 1 | Mem2.ndim > 1:
        raise Exception('Contingency: Requires two vector arguments')
    Cont = np.zeros((max(Mem1)+1, max(Mem2)+1))
    for i in range(len(Mem1)):
        x = Mem1[i]
        y = Mem2[i]
        try: 
            Cont[Mem1[i], Mem2[i]] = Cont[Mem1[i], Mem2[i]] + 1
        except Exception as e:
            print (e)
    return Cont
