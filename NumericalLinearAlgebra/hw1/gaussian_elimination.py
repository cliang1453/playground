import numpy as np
from utils import plot_solutions, plot_sol_comparison, plot_runtime_comparison
import time

class GaussianElimination(object):
	
	"""
	Implementation of a solver for non-homogeneous one-dimensional linear ODE in the form of 
	au^{''} + bu = cx^{2} + d, x \in [xb1, xb2], u(0) and u(1) bounary condition
	using centered difference scheme with specified number of discretization
	"""
	def __init__(self, num_discretization = None):
		super(GaussianElimination, self).__init__()
		self.num_discretization = num_discretization

	def descritize(self, n = 200, lam = 0, params = None):

		h = (params["xb2"] - params["xb1"]) / (n + 1)
		A = np.eye(n, k = 0) * (-2 * params["a"] / (h ** 2) + lam) \
		    + np.eye(n, k = 1) * params["a"] / (h ** 2) \
		    + np.eye(n, k = -1) * params["a"] / (h ** 2)

		b = params["c"] * np.arange(params["xb1"] + h, params["xb2"], h) ** 2 + params["d"]
		b[0] += params["u(0)"] / h**2
		b[-1] +=  params["u(1)"] / h**2
		return A, b

	def naive(self, A = None):
		
		m = A.shape[0]
		L, U = np.eye(m), np.copy(A)

		for k in range(m - 1): 
			L[k + 1:,k] = U[k + 1:,k] / U[k,k]
			for j in range(k + 1, m): 
				U[j,k:] -= L[j,k] * U[k,k:]

		return L, U

	def pivoting(self, A = None):
		
		m = A.shape[0]
		L, U, P = np.eye(m), np.copy(A), np.eye(m)

		for k in range(m-1):
			
			i = np.argmax(U[k:,k]) + k
			U[k,k:], U[i,k:] = U[i,k:], U[k,k:].copy()
			L[k,:k], L[i,:k] = L[i,:k], L[k,:k].copy()
			P[[k, i]] = P[[i, k]]

			L[k + 1:,k] = U[k + 1:,k] / U[k,k]
			for j in range(k + 1, m):
				U[j,k:] -= L[j,k] * U[k,k:]

		return L, U, P

	def forward(self, L = None, b = None):

		y = np.copy(b)
		for k in range(y.shape[0]):
			y[k] -= np.sum(L[k,:k] * y[:k])

		return y

	def backward(self, U = None, y = None):

		x = np.copy(y)
		for k in range(x.shape[0] - 1, -1, -1):
			x[k] -= np.sum(U[k,k + 1:] * x[k + 1:])
			x[k] = x[k] / U[k][k]

		return x

	def run(self, params = None, mode = "Pivoting"):

		xs = {} 
		times = {}

		for lam in params["b"]:
			
			xs[lam] = []
			times[lam] = []

			for n in self.num_discretization:
			
				A, b = self.descritize(n, lam, params)
				start = time.time()
				
				if mode == "Naive":
					L, U = self.naive(A)
				elif mode == "Pivoting":
					L, U, P = self.pivoting(A)
					b = np.matmul(P, b)
				else:
					ValueError("Mode not supported.")

				
				# forward substitution: Ly = b -> y
				y = self.forward(L = L, b = b)

				# backward substitution: Ux = y -> x
				x = self.backward(U = U, y = y)

				xs[lam].append(x)
				times[lam].append(time.time() - start)

		return xs, times
				

def main(num_discretization = None, params = None, exact_solution = None):

	# solver
	ge_solver = GaussianElimination(num_discretization = num_discretization)
	solution_naive, runtime_naive = ge_solver.run(params = params, mode = "Naive")
	solution_pivoting, runtime_pivoting = ge_solver.run(params = params, mode = "Pivoting")

	# plot
	plot_solutions(params, num_discretization, solution_naive, exact_solution, "Naive")
	plot_solutions(params, num_discretization, solution_pivoting, exact_solution, "Pivoting")
	
	plot_sol_comparison(params, num_discretization, solution_naive, solution_pivoting)
	plot_runtime_comparison(params, num_discretization, runtime_naive, runtime_pivoting)


if __name__ == '__main__':

	# params 
	num_discretization = [200, 400, 800, 1000, 2000, 4000]
	params = {"a": -1, "b": [0, 8], "c": 3, "d": -0.5, "xb1": 0, "xb2": 1, "u(0)": 1, "u(1)": -1}
	exact_solution = {0: [-0.25, 0.25, -2, 1], 
					  8: [0.375, 1.05556, -2.82843, -0.086805, 2.82843, 0.03125]}

	main(num_discretization, params, exact_solution)
		