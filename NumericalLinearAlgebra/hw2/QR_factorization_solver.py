import numpy as np
import math

class QRFactorizationSolver(object):
	
	"""
	QR factorization solver class. Implement methods include
	1. classical gram schmidt
	2. shifted gram schmidt 
	3. householder triangularization 
	4. householder triangularization with input as banded matrix
	"""

	def __init__(self):
		super(QRFactorizationSolver, self).__init__()

	def classicalGramSchmidt(self, A = None):

		N, M = A.shape
		Q = A.copy()
		R = np.zeros(A.shape)

		for j in range(N):
			v = A[j]
			for i in range(j):
				R[i,j] = np.dot(Q[i], A[j])
				v = v - R[i, j] * Q[i]
			R[j, j] = np.linalg.norm(v, 2)
			Q[j] = v / R[j, j]

		return Q, R

	def modifiedGramSchmidt(self, A = None):

		N, M = A.shape
		V = A.copy()
		Q = A.copy()
		R = np.zeros(A.shape)

		for j in range(N):


	def householderTriangularization(self, A = None, upper_band = math.inf, lower_band = math.inf):

		N, M = A.shape
	
		for k in range(1, N):

			n = min(k+lower_band, N)
			m = min(k+upper_band, M)

			x = A[k:n, k]
			e = np.zeros(x.shape)
			e[0] = 1
			v = np.sign(x[0]) * np.linalg.norm(x, 2) * e + x
			v = v / np.linalg.norm(v, 2)
			A[k:n, k:m] = A[k:n, k:m] - 2 * np.matmul(np.outer(v, v), A[k:n, k:m])

		return A







		
		