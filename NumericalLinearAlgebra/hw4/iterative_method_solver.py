import numpy as np

class IterativeMethodSolver(object):
	
	"""
	Class for solving linear system Ax=b using iterative methods
	1. Jacobi Iteration
	2. Gauss-Seidel Iteration
	3. Sucessive Over Relaxation Iteration (SOR)
	3. Steepest Descent Method
	4. Conjugate Gradient Method
	"""

	def __init__(self, max_iter=100):
		super(IterativeMethodSolver, self).__init__()
		self.max_iter = max_iter

	def jacobi(self, A, b):
		
		D = np.diag(np.diag(A))
		G = np.matmul(np.linalg.pinv(D), D-A)
		f = np.matmul(np.linalg.pinv(D), b)
		x = np.zeros(A.shape[0])

		for _ in range(self.max_iter):
			x = np.matmul(G, x) + f

		return x

	def gauss_seidel(self, A, b):

		DE = np.tril(A)
		G = np.matmul(np.linalg.pinv(DE), DE - A)
		f = np.matmul(np.linalg.pinv(DE), b)
		x = np.zeros(A.shape[0])

		for _ in range(self.max_iter):
			x = np.matmul(G, x) + f

		return x

	def sor(self, A, b, w=1.5):

		# w: [0, 1] intrapolation, [1, 2] extrapolation
		DE = np.tril(A)
		G = np.matmul(np.linalg.pinv(DE), DE - A)
		f = np.matmul(np.linalg.pinv(DE), b)
		x = np.zeros(A.shape[0])

		for _ in range(self.max_iter):
			z = np.matmul(G, x) + f
			x = w * z + (1-w) * x

		return x

	def steepest_descent(self, A, b):
		
		x = np.zeros(A.shape[0])

		for _ in range(self.max_iter):
			r = b - np.matmul(A, x)
			alpha = np.inner(r, r)/np.inner(r, np.matmul(A, r))
			x += alpha * r

		return x

	def conjugate_gradient(self, A, b):
		
		x = np.zeros(A.shape[0])
		r = b.copy()
		p = b.copy()

		for _ in range(self.max_iter):
			alpha = np.inner(r, r)/np.inner(p, np.matmul(A, p))
			x = x + alpha * p
			r_new = r - alpha * np.matmul(A, p)
			beta = np.inner(r_new, r_new)/np.inner(r, r)
			p = r_new + beta * p
			r = r_new

		return x
