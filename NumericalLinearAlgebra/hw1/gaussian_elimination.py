import numpy as np
from utils import plot
import time

class GaussianElimination(object):
	"""docstring for GaussianElimination"""
	def __init__(self, num_discretization = None):
		super(GaussianElimination, self).__init__():
		self.num_discretization = num_discretization

	def naive(self, A = None):
		return L, U

	def pivoting(self, A = None):
		return L, U

	def descritize(self, n = 200, lam = 0):
		return A

	def forward(self, L = None, b = None):
		return y

	def backward(self, U = None, y = None):
		return x

	def run(self, lambda_choices = None, mode = "pivoting"):

		xs = {}
		time = {}
		for lam in lambda_choices:
			
			xs[lam] = []
			time[lam] = []
			for n in self.num_discretization:
			
				A, b = self.descritize(n, lam)
				
				if mode == "naive":
					L, U = self.naive(A)
				elif mode == "pivoting":
					L, U = self.pivoting(A)

				# forward substitution: Ly = b -> y
				y = self.forward(L = L, b = b)

				# backward substitution: y = Ux -> x
				x = self.backward(U = U, y = y)
				xs[lam].append(x)

		return xs
				

def main():

	# params
	num_discretization = [200, 400, 800, 1000, 2000, 4000]
	lambda_choices = [0, 8]

	# solver
	ge_solver = GaussianElimination(num_discretization = num_discretization)
	xs_naive, time_naive = ge_solver.run(lambda_choices = lambda_choices, mode = "naive")
	xs_pivoting, time_pivoting = ge_solver.run(lambda_choices = lambda_choices, mode = "pivoting")

	#
	plot




if __name__ == '__main__':
	main()
		