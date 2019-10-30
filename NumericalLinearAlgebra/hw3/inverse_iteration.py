import numpy as np

def main():
	
	mu = 2.5
	A = np.array([[4, 2, 0],[2, 6, 2],[0, 2, 8]])
	I = np.eye(3)
	A_prime_inv = np.linalg.inv(A - mu * I)
	v = np.ones(3)
	v = v/np.linalg.norm(v, 2)
	u, sigma, vh = np.linalg.svd(A, full_matrices=True)
	
	for k in range(5):
		print("=====starting of " + str(k) + " iteration=======")
		w = np.matmul(A_prime_inv, v)
		v = w/np.linalg.norm(w, 2)
		print(v)
		print(np.linalg.norm(v - u[:, -1], 2))
		lamda = np.matmul(np.matmul(v.T, A), v)
		print(lamda)

if __name__ == '__main__':
	main()