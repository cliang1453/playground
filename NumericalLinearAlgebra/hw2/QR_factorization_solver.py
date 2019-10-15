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

		M, N = A.shape
		Q = A.copy()
		R = np.zeros(A.shape)

		for j in range(N):
			v = A[:, j]
			
			for i in range(j):
				R[i,j] = np.dot(Q[:, i], A[:, j])
				v = v - R[i, j] * Q[:, i]
			
			R[j, j] = np.linalg.norm(v, 2)
			Q[:, j] = v / R[j, j]

		return Q, R

	def modifiedGramSchmidt(self, A = None):

		M, N = A.shape
		V = A.copy()
		Q = np.zeros(A.shape)
		R = np.zeros(A.shape)

		for i in range(N):
			R[i, i] = np.linalg.norm(V[:, i], 2)
			Q[:, i] = V[:, i]/R[i, i]
			
			for j in range(i + 1, N):
				R[i, j] = np.dot(Q[:, i], V[:, j])
				V[:, j] = V[:, j] - R[i, j] * Q[:, i]

	def householderTriangularization(self, A = None, upper_band = math.inf, lower_band = math.inf):

		M, N = A.shape
		
		for k in range(N):

			m = min(k + lower_band, M)
			n = min(k + upper_band, N)

			x = A[k:m, k]
			e = np.zeros(x.shape)
			e[0] = 1
			v = np.sign(x[0]) * np.linalg.norm(x, 2) * e + x
			v = v / np.linalg.norm(v, 2)
			A[k:m, k:n] = A[k:m, k:n] - 2 * np.matmul(np.outer(v, v), A[k:m, k:n])

		return A







		
		