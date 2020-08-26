import matplotlib
matplotlib.use('Agg')
import numpy as np
import matplotlib.pyplot as plt

def lagrange_polynomial(P_list):

	data_t_list = np.linspace(-1, 1, 200)
	data_y_list = 1/(1 + 25 * data_t_list**2)
	
	for P in P_list:
		
		t_list = np.linspace(-1, 1, P+1) # [P+1,]
		y_list = 1/(1 + 25 * t_list**2) # [P+1,]

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

		plt.plot(data_t_list, data_y_list, label="f(t)")
		plt.plot(data_t_list, f, label=str(P) + "-th order polynomial")
		plt.legend()
		plt.savefig("p4_" + str(P) + ".png")
		plt.close()


if __name__ == '__main__':
	P_list = [3,5,7,9,11,15]
	lagrange_polynomial(P_list)