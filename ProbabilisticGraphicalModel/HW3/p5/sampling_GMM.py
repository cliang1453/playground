import numpy as np
import matplotlib
import matplotlib.pyplot as plt

def sampling_gmm(mu, sigma, weight, N):
	z = np.random.binomial(1, weight, N)
	X = np.random.normal(mu[0], sigma[0], N) * (1-z) \
		    + np.random.normal(mu[1], sigma[1], N) * z
	return X

def metropolis_hastings_sampling(mu_0, sigma, burn_in_samples, collect_samples, X):

	# Q(mu_new | mu)
	def Q(mu, N):
		return np.exp(-1*(mu[0]**2 + mu[1]**2)/(2*N)) 

	# P(mu)
	def P(mu, X): 
		return np.prod(0.5*np.exp(-1*(X-mu[0])**2/2) + 0.5*np.exp(-1*(X-mu[1])**2/2))
	
	mu = mu_0
	mu_est = mu_0
	mu1_samples, mu2_samples = [], []
	acc_rate = 0

	for t in range(burn_in_samples + collect_samples):
		
		# mu_new \sim Q(mu_new | mu)
		mu_new = np.random.multivariate_normal(mu, [[sigma**2, 0], [0, sigma**2]], 1)[0]

		# A(mu_new | mu) = min(1, P(mu_new)Q(mu|mu_new) / P(mu)Q(mu_new|mu))
		A = min(1, P(mu_new, X) * Q(mu_new, len(X)) / P(mu, X) / Q(mu, len(X)))
		
		# accept the sample
		if np.random.binomial(1, A, 1) == 1:
			acc_rate += 1
			mu = mu_new

		if t >= burn_in_samples:
			mu_est += mu
			mu1_samples.append(mu[0])
			mu2_samples.append(mu[1])

	mu_est /= collect_samples
	acc_rate /= (burn_in_samples + collect_samples)

	# axi = fig.add_subplot(2,3,i+1)
	# axi.scatter(u1_samples, mu2_samples, color='b')
	# axi.set_xlabel('mu_1')
	# axi.set_ylabel('mu_2')
	return acc_rate, mu_est
	
def gibbs_sampling(mu_0, burn_in_samples, collect_samples, X):

	mu = mu_0
	mu_est = mu_0
	mu1_samples, mu2_samples = [], []

	for t in range(burn_in_samples + collect_samples):
		
		p0 = np.exp(-0.5*(X - mu[0])**2)
		p1 = np.exp(-0.5*(X - mu[1])**2)
		z = np.random.binomial(1, p1/(p0+p1), len(X))

		mu_cov = np.array([[1/(len(X)- np.sum(z) + 1), 0],\
						   [0, 1/(np.sum(z) + 1)]])
		mu_mu = np.array([np.sum(X[z==0])/(len(X) - np.sum(z) + 1), \
						  np.sum(X[z==1])/(np.sum(z) + 1)])

		mu = np.random.multivariate_normal(mu_mu, mu_cov, 1)[0]

		if t >= burn_in_samples:
			mu_est += mu
			mu1_samples.append(mu[0])
			mu2_samples.append(mu[1])

	mu_est /= collect_samples

	# axi = fig.add_subplot(2,3,i+1)
	# axi.scatter(u1_samples, mu2_samples, color='b')
	# axi.set_xlabel('mu_1')
	# axi.set_ylabel('mu_2')
	return mu_est
	

	

def main():

	X = sampling_gmm(mu=[-5, 5], sigma=[1, 1], weight=0.5, \
		N=100)

	MH sampling
	print('=============MH sampling============')
	for sig in [0.5, 5]:
		print('=========sigma=' + str(sig) + '==========')
		#fig = plt.figure()
		for i in range(6):
			acc_rate, mu_est = metropolis_hastings_sampling(\
				mu_0=np.array([0.0, 0.0]), sigma=sig, \
				burn_in_samples=10000, collect_samples=1000, \
				X=X)
			print("mu1: " + str(mu_est[0]) + " mu2: " + str(mu_est[1]) + \
				" acc rate: " + str(acc_rate))
		fig.savefig('mh' + str(sig) + '.png')
			
	# Gibbs sampling
	print('=============Gibbs sampling============')
	# fig = plt.figure()
	for i in range(6):
		mu_est = gibbs_sampling(mu_0=np.array([0.0, 0.0]), \
			burn_in_samples=10000, collect_samples=1000, \
			X=X)
		print("mu1: " + str(mu_est[0]) + " mu2: " + str(mu_est[1]))
	# fig.savefig('gibbs.png')


	




if __name__ == '__main__':
	main()