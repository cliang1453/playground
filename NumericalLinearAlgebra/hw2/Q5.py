import time
import numpy as np
from QR_factorization_solver import QRFactorizationSolver
from utils import *

def gen_matrix(sin_vals, n):
	
	Sigma = np.diag(sin_vals)
	U = np.random.rand(n, n) 
	V = np.random.rand(n, n)

	for i in range(n):
		for j in range(i):
			U[:, i] -= U[:, j]
			V[:, i] -= V[:, j]

		U[:,i] = U[:,i] / np.linalg.norm(U[:,i])
		V[:,i] = V[:,i] / np.linalg.norm(V[:,i])

	return np.matmul(np.matmul(U, Sigma), V)

def main(n_range):

	solver = QRFactorizationSolver()
	times = {"classical": [], "modified": []}

	for n in n_range:
		
		sin_vals = np.array([3**(-i) for i in range(1, n+1)])
		n_times = {"classical": [], "modified": []}

		for _ in range(10):

			A = gen_matrix(sin_vals, n)
			
			start = time.time()
			solver.classicalGramSchmidt(A)
			n_times["classical"].append(time.time() - start)

			start = time.time()
			solver.modifiedGramSchmidt(A)
			n_times["modified"].append(time.time() - start)

		times["classical"].append(np.mean(n_times["classical"]))
		times["modified"].append(np.mean(n_times["modified"]))


	plot_runtime_comparison(n_range, times["classical"], times["modified"])



	
if __name__ == '__main__':
	n_range = [100, 200, 300]
	main(n_range)