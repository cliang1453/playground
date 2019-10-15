import time
import numpy as np
from QR_factorization_solver import QRFactorizationSolver
from utils import *
import math

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
	times = {"classical": [], "modified": [], "householder": []}
	R_error = {"classical": [], "modified": [], "householder": []}

	for n in n_range:
		
		sin_vals = np.array([3**(-i) for i in range(1, n+1)])
		n_times = {"classical": [], "modified": [], "householder": []}
		n_R_error = {"classical": [], "modified": [], "householder": []}

		for _ in range(10):

			A = gen_matrix(sin_vals, n)
			_, true_R = np.linalg.qr(A)
			
			start = time.time()
			_, R = solver.classicalGramSchmidt(A)
			n_times["classical"].append(time.time() - start)
			n_R_error["classical"].append(np.linalg.norm(R - true_R, 'fro'))

			start = time.time()
			_, R = solver.modifiedGramSchmidt(A)
			n_times["modified"].append(time.time() - start)
			n_R_error["modified"].append(np.linalg.norm(R - true_R, 'fro'))

			start = time.time()
			R = solver.householderTriangularization(A, math.inf, math.inf)
			n_times["householder"].append(time.time() - start)
			n_R_error["householder"].append(np.linalg.norm(R - true_R, 'fro'))


		times["classical"].append(np.mean(n_times["classical"]))
		times["modified"].append(np.mean(n_times["modified"]))
		times["householder"].append(np.mean(n_R_error["householder"]))
		R_error["classical"].append(np.mean(n_R_error["classical"]))
		R_error["modified"].append(np.mean(n_R_error["modified"]))
		R_error["householder"].append(np.mean(n_R_error["householder"]))


	plot_runtime_comparison(n_range, times)
	plot_error_comparison(n_range, R_error)



	
if __name__ == '__main__':
	n_range = [100, 200, 300]
	main(n_range)