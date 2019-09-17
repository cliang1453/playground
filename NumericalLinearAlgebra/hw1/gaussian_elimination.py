import numpy as np
from utils import plot_solutions, plot_runtime, plot_comparison
import time

class GaussianElimination(object):
	"""docstring for GaussianElimination"""
	def __init__(self, num_discretization = None):
		super(GaussianElimination, self).__init__():
		self.num_discretization = num_discretization

	def descritize(self, n = 200, lam = 0, params = None):

		h = (params["xb1"] - params["xb2"]) / (n + 1)
		A = np.eye(n, k = 0) * (-2 * params["a"] / (h ** 2) + lam) \
		    + np.eye(n, k = 1) * params["a"] / (h ** 2) \
		    + np.eye(n, k = -1) * params["a"] / (h ** 2)

		b = params["c"] * np.array([for x**2 in x in range(xb1, xb2, h)]) + params["d"]
		return A, b

	def naive(self, A = None):
		
		m = A.shape[0]
		L, U = np.ones(m), np.copy(A)

		for k in range(m - 1): 
			
			L[k + 1:][k] = U[k + 1:][k] / U[k][k]
			for j in range(k + 1, m): 
				U[j][k:] -= L[j][k] * U[k][k:]

		return L, U

	def forward(self, L = None, b = None):

		y = np.copy(b)
		for k in range(y.shape[0]):
			y[k] -= np.sum(L[:k][k] * y[:k])

		return y

	def backward(self, U = None, y = None):

		x = np.copy(y)
		for k in range(x.shape[0] - 1, -1, -1):
			x[k] -= np.sum(U[k + 1:][k] * x[k + 1:])

		return x

	def pivoting(self, A = None):
		
		m = A.shape[0]
		L, U, P = np.ones(m), np.copy(A), np.ones(m)

		for k in range(m-1):
			
			i = np.argmax(U[k:][k]) + k
			U[k][k:], U[i][k:] = U[i][k:], U[k][k:].copy()
			L[k][:k], L[i][:k] = L[i][:k], L[k][:k].copy()
			P[[k, i]] = P[[i, k]]

			L[k + 1:][k] = U[k + 1:][k] / U[k][k]
			for j in range(k + 1, m):
				U[j][k:] -= L[j][k] * U[k][k:]

		return L, U, P

	def run(self, params = None, mode = "Pivoting"):

		xs = {} 
		times = {}

		for lam in params["b"]:
			
			xs[lam] = []
			times[lam] = []

			for n in self.num_discretization:
			
				A, b = self.descritize(n, lam, params)
				start = time.perf_counter()
				
				if mode == "Naive":
					L, U = self.naive(A)
				elif mode == "Pivoting":
					L, U, P = self.pivoting(A)
					A = np.matmul(P, A)
					b = np.matmul(P, b)

				# forward substitution: Ly = b -> y
				y = self.forward(L = L, b = b)

				# backward substitution: Ux = y -> x
				x = self.backward(U = U, y = y)

				xs[lam].append(x)
				times[lam].append(time.time() - start)

		return xs, times
				

def main():

	# params 
	num_discretization = [200, 400, 800, 1000, 2000, 4000]
	# params for solving the non-homogeneous one-dimensional ODE in form of 
	# au^{''} + bu = cx^{2} + d, x \in [xb1, xb2]
	# using centered difference scheme
	params = {"a": -1, "b": [0, 8], "c": 3, "d": -0.5, "xb1": 0.0, "xb2": 1.0}

	# solver
	ge_solver = GaussianElimination(num_discretization = num_discretization)
	solution_naive, runtime_naive = ge_solver.run(params = params, mode = "Naive")
	solution_pivoting, runtime_pivoting = ge_solver.run(params = params, mode = "Pivoting")

	plot_solutions(params, num_discretization, solution_naive, exact_solution, "Naive")
	plot_solutions(params, num_discretization, solution_pivoting, exact_solution, "Pivoting")
	
	plot_sol_comparison(params, num_discretization, solution_naive, solution_pivoting)
	plot_runtime_comparison(params, num_discretization, runtime_naive, runtime_pivoting)


if __name__ == '__main__':
	main()
		