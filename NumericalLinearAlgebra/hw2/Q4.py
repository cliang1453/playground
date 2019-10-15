import scipy
from scipy import linalg
import time
import numpy as np
from QR_factorization_solver import QRFactorizationSolver
from utils import *


def main(m_range):

	solver = QRFactorizationSolver()
	times = []

	for m in m_range:
		A = scipy.linalg.toeplitz([9, 3, 2, 1] + [0] * (m-4), [9, 3, 2, 1] + [0] * (m-4))
		start = time.time()
		R = solver.householderTriangularization(A = A, 
			upper_band = 3, lower_band = 3)
		times.append(time.time() - start)

	plot_runtime(m_range, times)


	


if __name__ == '__main__':
	m_range = [10, 100, 1000, 10000]
	main(m_range)