import matplotlib
matplotlib.use('Agg')
import numpy as np
import matplotlib.pyplot as plt


def b2(t):
	if -1.5 < t <= -0.5:
		return (t+1.5)**2/2
	elif -0.5 < t <= 0.5:
		return -t**2 + 3/4
	elif 0.5 < t <= 1.5:
		return (t-1.5)**2/2
	else:
		return 0

def solve_alpha(f_vals):
	T = np.repeat(np.linspace(0,4,5)[:,None],5,axis=1)
	b_T_k = T - T.transpose()
	for r in range(len(b_T_k)):
		for c in range(len(b_T_k[0])):
			b_T_k[r][c] = b2(b_T_k[r][c])
	alpha = np.linalg.inv(b_T_k).dot(f_vals)
	print(alpha)

def piecepoly2(alpha):
	t_list = np.linspace(-10, 15, 3000)
	f_list = [0]*3000
	for idx in range(len(t_list)):
		t = t_list[idx]
		for k in range(5):
			f_list[idx] += alpha[k] * b2(t-k)

	plt.plot(t_list, f_list)
	plt.savefig("p5a.png")
	plt.close()


if __name__ == '__main__':
	alpha = np.array([3,2,-1,4,-1])
	f_vals = np.array([1,2,-4,-5,-2])
	# piecepoly2(alpha)
	solve_alpha(f_vals)