import matplotlib
matplotlib.use('Agg')
import numpy as np
import matplotlib.pyplot as plt
import scipy
import scipy.io 

def trigonometric_polynomial(t_list, y_list, P):

	T = np.ones([len(t_list), 2*P+1])
	for k in range(1, P+1):
		T[:,k] = np.cos(2*np.pi*k*t_list)
	for k in range(P+1, 2*P+1):
		T[:,k] = np.sin(2*np.pi*k*t_list)
	alpha = np.linalg.inv(T).dot(y_list)

	data_t_list = np.linspace(0, 1, 500)
	data_T = np.ones([len(data_t_list), 2*P+1])
	for k in range(1, P+1):
		data_T[:,k] = np.cos(2*np.pi*k*data_t_list)
	for k in range(P+1, 2*P+1):
		data_T[:,k] = np.sin(2*np.pi*k*data_t_list)

	f = data_T.dot(alpha)
	plt.scatter(t_list, y_list, label="data points")
	plt.plot(data_t_list, f, label=str(P) + "-th order trigonometric polynomial")
	plt.legend()
	plt.savefig("p6b.png")
	plt.close()


def lagrange_polynomial(t_list, y_list, P):

	data_t_list = np.linspace(0, 1, 500)
	
	f = [0]*len(data_t_list)
	for idx in range(len(data_t_list)):
		t = data_t_list[idx]
		# compute f(t)
		for k in range(P+1):
			# compute y_k, t_k
			yk = y_list[k]
			tk = t_list[k]
			# compute P_k(t)
			numer = np.repeat(t, P) - np.concatenate((t_list[:k], t_list[k+1:]))
			denom = np.repeat(tk, P) - np.concatenate((t_list[:k], t_list[k+1:]))
			Pkt = np.prod(numer/denom)
			f[idx] += yk * Pkt

	plt.scatter(t_list, y_list, label="data points")
	plt.plot(data_t_list, f, label=str(P) + "-th order lagrange polynomial")
	plt.legend()
	plt.savefig("p6a.png")
	plt.close()

if __name__ == '__main__':

	mat = scipy.io.loadmat("hw01p6_nonuniform_samples.mat")
	P = 10
	lagrange_polynomial(mat['t'].squeeze(), mat['y'].squeeze(), P)
	P = 5
	trigonometric_polynomial(mat['t'].squeeze(), mat['y'].squeeze(), P)