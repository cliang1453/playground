import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import multivariate_normal


 
'''Method 1: random walk with multivariate normal'''
# N = 100
# x = np.zeros([N,2])

# # target
# p_mu = np.array([1.0, 2.0])
# p_cov = np.array([[1, 0.9], [0.9, 1]])
# p = multivariate_normal(p_mu, p_cov)

# # proposal
# q_cov = np.array([[1.0, 0.0], [0.0, 1.0]])
# ac = 0.0
# for t in range(1, N):
    
#     #xStar = np.random.multivariate_normal(x[t-1,:], q_cov) # 0.39
#     xStar = np.random.multivariate_normal([0.0, 0.0], q_cov) # 0.25
#     alpha = min(1, p.pdf(xStar)/p.pdf(x[t-1,:]))

#     u = np.random.uniform(low=0.0, high=1.0)
#     if u < alpha:
#         x[t,:] = xStar
#         ac += 1
#     else:
#         x[t,:] = x[t-1,:]
# print(ac/N)
# # 0.39

# plt.figure()
# p1, p2 = x.T
# plt.scatter(p1, p2)

# plt.show()

# t1, t2 = np.mgrid[-1:3:0.1, 0:4:0.1]
# pos = np.empty(t1.shape + (2,))
# pos[:, :, 0] = t1
# pos[:, :, 1] = t2
# plt.contourf(t1, t2, p.pdf(pos))

# plt.show()


'''Method 2: random walk with multivariate student T'''

def multivariate_t_rvs(m, S, df=np.inf, n=1):
    '''generate random variables of multivariate t distribution
    Parameters
    ----------
    m : array_like
        mean of random variable, length determines dimension of random variable
    S : array_like
        square array of covariance  matrix
    df : int or float
        degrees of freedom
    n : int
        number of observations, return random array will be (n, len(m))
    Returns
    -------
    rvs : ndarray, (n, len(m))
        each row is an independent draw of a multivariate t distributed
        random variable
    '''
    m = np.asarray(m)
    d = len(m)
    if df == np.inf:
        x = 1.
    else:
        x = np.random.chisquare(df, n)/df
    z = np.random.multivariate_normal(np.zeros(d),S)
    return m + z/np.sqrt(x)# same output format as random.multivariate_normal


N = 100
x = np.zeros([N,2])

# target
p_mu = np.array([1.0, 2.0])
p_cov = np.array([[1, 0.9], [0.9, 1]])
p = multivariate_normal(p_mu, p_cov)

# proposal
q_cov = np.array([[1.0, 0.0], [0.0, 1.0]])
ac = 0.0
for t in range(1, N):
    
    xStar = multivariate_t_rvs(x[t-1,:], q_cov, 10, 2) #0.34
    # xStar = multivariate_t_rvs([0, 0], q_cov, N, 2) #0.21
    alpha = min(1, p.pdf(xStar)/p.pdf(x[t-1,:]))

    u = np.random.uniform(low=0.0, high=1.0)
    if u < alpha:
        x[t,:] = xStar
        ac += 1
    else:
        x[t,:] = x[t-1,:]
print(ac/N)
# 0.31

plt.figure()
p1, p2 = x.T
plt.scatter(p1, p2)

plt.show()

t1, t2 = np.mgrid[-1:3:0.1, 0:4:0.1]
pos = np.empty(t1.shape + (2,))
pos[:, :, 0] = t1
pos[:, :, 1] = t2
plt.contourf(t1, t2, p.pdf(pos))

plt.show()