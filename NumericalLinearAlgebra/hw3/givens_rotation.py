import numpy as np
from utils import *
import time

# return the rotation matrix
def get_rotation(i, j, theta, n):
	
	G = np.eye(n)
	G[i][i] = np.cos(theta)
	G[i][j] = -np.sin(theta)
	G[j][i] = np.sin(theta)
	G[j][j] = np.cos(theta)

	return G

def main(N = 100):

	times = []
	for n in N:
		
		A = np.diag(np.random.rand(n))
		A[1:, 0] = np.random.rand(n-1)
		A[0, 1:] = np.random.rand(n-1)
		# Q = np.eye(n)

		# from column 0 -> n-1
		# from row k -> n-1

		start = time.time()
		for c in range(n):
			for r in range(c + 1, n):
				theta = np.arctan2(- A[r][c], A[c][c])
				G = get_rotation(c, r, theta, n)
				# Q = np.matmul(G, Q)
				A = np.matmul(G, A)
		times.append(time.time() - start)
		print(n)
	plot_runtime(m_range = N, times = times)



if __name__ == '__main__':
	N = [100, 200, 400, 800]
	main(N)