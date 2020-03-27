import scipy.io as sio
import numpy as np
import matplotlib.pyplot as plt

def main(W, b, p, s, D, S):

	K = 50 # number of chains
	T = 100000 # 1000D burn-in time,
	d = np.eye(K, D) # inital states of d for K chains
	p = np.squeeze(p, axis=1)
	b = np.squeeze(b, axis=1)
	s = np.squeeze(s, axis=1)

	for t in range(T):
		for k in range(K):
			
			i = t%D
			# p(d_i = 0 | d_{-i}, s) \propto p(d_i=0, d_{-1}, s)
			d[k][i] = 0
			pi0 = np.prod((1 + np.exp(-np.matmul(W, d[k])-b))**(-1*s)) \
				* np.prod((1 + np.exp( np.matmul(W, d[k])+b))**(-1+s)) \
				* (1-p[i])

			# p(d_i = 1 | d_{-i}, s) \propto p(d_i=1, d_{-1}, s)
			d[k][i] = 1
			pi1 = np.prod((1 + np.exp(-np.matmul(W, d[k])-b))**(-1*s)) \
				* np.prod((1 + np.exp(np.matmul(W, d[k])+b))**(-1+s)) \
				* p[i]

			# assign new values to d
			d[k][i] = np.random.binomial(1, pi1/(pi0+pi1))

		if t%D == 0:
			Ed = np.mean(d, axis=0)
			print("t=" + str(t) + ": " + str(np.log(np.sum(Ed))))
			with np.printoptions(precision=3):
				print(Ed)
			

if __name__ == '__main__':
	mat = sio.loadmat('SymptomDisease.mat')
	
	W = mat['W'] # [200, 50]
	b = mat['b'] # [200, 1]
	p = mat['p'] #[50, 1]
	s = mat['s'] #[200, 1]
	D = p.shape[0]
	S = s.shape[0]

	main(W, b, p, s, D, S)