import numpy as np 
from gaussian_elimination import GaussianElimination
import time
import numpy as np
from iterative_method_solver import IterativeMethodSolver
from utils import *

def main(params, m_range, lambda_range, methods, max_iter):
	
	ge_solver = GaussianElimination(num_discretization = m_range)
	As, bs = ge_solver.get_Ab(params = params) # [lambda][size]

	iter_solver = IterativeMethodSolver(max_iter=max_iter)
	
	times, err = {}, {}
	for method in methods:
		times[method] = {lamda: [] for lamda in lambda_range}
		err[method] = {lamda: [] for lamda in lambda_range}


	for m in m_range:
		for lamda in lambda_range:
			
			A, b = As[lamda][m], bs[lamda][m]
			x_true = np.matmul(np.linalg.inv(A), b)
			
			for method in methods:
			
				start = time.time()
				
				if method == "steepest_descent":
					x = iter_solver.steepest_descent(A=A, b=b)
				elif method == "conjugate_gradient":
					x = iter_solver.conjugate_gradient(A=A, b=b)
				else:
					print("Method not supported")

				times[method][lamda].append(time.time() - start)
				err[method][lamda].append(np.linalg.norm(x - x_true, 2))

	plot_runtime_method(m_range, lambda_range, methods, times)
	plot_runtime_mu(m_range, lambda_range, methods, times)
	plot_err_method(m_range, lambda_range, methods, err)
	plot_err_mu(m_range, lambda_range, methods, err)

if __name__ == '__main__':
	
	params = {"a": -1, "b": [0, 2], "c": 3, "d": -0.5, "xb1": 0, "xb2": 1, "u(0)": 1, "u(1)": -1}
	m_range = [100, 200, 400, 800]
	lambda_range = [0, 2]
	methods = ["steepest_descent", "conjugate_gradient"]
	max_iter = 1000
	main(params, m_range, lambda_range, methods, max_iter)