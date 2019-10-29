import numpy as np
def main():
	A = np.array([[-3, 3, 7, 2],[1, 2, 3, -5],[2, -1, 0, 3], [4, 2, -2, 4]])
	m = 4
	I = np.eye(m)

	for k in range(m-2):
		x = A[k+1:, k]
		e = np.zeros(x.shape)
		e[0] = 1
		v = np.sign(x[0]) * np.linalg.norm(x, 2) * e + x
		v = v / np.linalg.norm(v, 2)
		v = np.pad(v, (k+1,0), 'constant', constant_values=(0,0))
		print(v)
		Q = (I - 2 * np.outer(v, v) / np.dot(v, v))
		print(Q)
		A = np.matmul(np.matmul(Q.T, A), Q)
		print(A)
		print("===========")




if __name__ == '__main__':
	main()