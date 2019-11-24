import scipy
from scipy import linalg
import time
import numpy as np
from iterative_method_solver import IterativeMethodSolver
from utils import *


def main(m_range, mu_range, methods, max_iter, w):

	solver = IterativeMethodSolver(max_iter=max_iter)
	
	times, err = {}, {}
	for method in methods:
		times[method] = {mu : [] for mu in mu_range}
		err[method] = {mu : [] for mu in mu_range}

	for m in m_range:
		
		T = scipy.linalg.toeplitz([9, 3, 2, 1] + [0] * (m-4), [9, 3, 2, 1] + [0] * (m-4))
		I = np.eye(m)
		b = np.random.rand(m)
		
		for mu in mu_range:
			
			A = T + mu * I
			x_true = np.matmul(np.linalg.inv(A), b)

			for method in methods:
			
				start = time.time()
				
				if method == "jacobi":
					x = solver.jacobi(A=A, b=b)
				elif method == "gauss_seidel":
					x = solver.gauss_seidel(A=A, b=b)
				elif method == "sor":
					x = solver.sor(A=A, b=b, w=w)
				else:
					print("Method not supported")

				times[method][mu].append(time.time() - start)
				err[method][mu].append(np.linalg.norm(x - x_true, 2))



	plot_runtime_method(m_range, mu_range, methods, times)
	plot_runtime_mu(m_range, mu_range, methods, times)
	plot_err_method(m_range, mu_range, methods, err)
	plot_err_mu(m_range, mu_range, methods, err)

if __name__ == '__main__':
	m_range = [100, 200, 400, 800]
	mu_range = [10, -10]
	methods = ["jacobi", "gauss_seidel", "sor"]
	max_iter = 1000
	w = 1.5
	main(m_range, mu_range, methods, max_iter, w)