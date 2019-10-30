from gaussian_elimination import GaussianElimination
from QR_factorization_solver import QRFactorizationSolver
import numpy as np
import time
from utils import plot_runtime_comparison

# Use pure QR Algorithm to compute eigenvalues
def QR_Algorithm(A = None, num_iter = 100, qr_solver= None):
	
	for k in range(num_iter): 
		Q, R = qr_solver.modifiedGramSchmidt(A)
		A = np.matmul(R, Q)

	return A.diagonal()

def main(num_discretization = None, params = None):
	
	ge_solver = GaussianElimination(num_discretization = num_discretization)
	As, _ = ge_solver.get_Ab(params = params)
	qr_solver = QRFactorizationSolver()
	times = {}

	for lam, vs in As.items():
		times[lam] = []
		for n, A in vs.items():
			
			start = time.time()
			eigen_val = QR_Algorithm(A = A, qr_solver = qr_solver)
			times[lam].append(time.time() - start)
			abs_eigen_val = np.abs(eigen_val)
			kappa = np.amax(abs_eigen_val) / np.amin(abs_eigen_val)

	for lam, vs in As.items():
		plot_runtime_comparison(As, times, params, num_discretization)



if __name__ == '__main__':
	num_discretization = [256, 512, 1024]
	params = {"a": -1, "b": [0, 2], "c": 3, "d": -0.5, "xb1": 0, "xb2": 1, "u(0)": 1, "u(1)": -1}
	main(num_discretization, params)