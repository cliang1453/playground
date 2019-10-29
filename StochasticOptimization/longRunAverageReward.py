import numpy as np

def main(R = None, P = None, N = 5000, D_len = 4, S_len = 2):

	P_star = np.zeros((D_len, S_len, S_len))
	G = np.zeros((D_len, S_len))
	B = np.zeros((D_len, S_len))
	I = np.eye(S_len)

	for d in range(D_len):
		
		for t in range(N):
			P_star[d] += np.linalg.matrix_power(P[d], t)
		
		P_star[d] /= N
		G[d] = np.dot(P_star[d], R[d])
		Hp = np.matmul(np.linalg.inv(I - P[d] + P_star[d]), I - P_star[d])
		B[d] = np.dot(Hp, R[d])

	print("Gain: ")
	print(G)

	print("Bias: ")
	print(B)

if __name__ == '__main__':
	
	S_len = 2 
	A_len = 2
	D_len = 4

	# R[d][s]: reward r(s, a) under stationary policy d
	R = np.zeros((D_len, S_len))
	R[0] = np.array([10, 25])
	R[1] = np.array([5, 25])
	R[2] = np.array([10, 35])
	R[3] = np.array([5, 35])

	# P[d][s][j]: transition probability p(j | s, a) under stationary policy d
	P = np.zeros((D_len, S_len, S_len))
	P[0] = np.array([[0.5, 0.5], [0.6, 0.4]])
	P[1] = np.array([[0.3, 0.7], [0.6, 0.4]])
	P[2] = np.array([[0.5, 0.5], [0.2, 0.8]])
	P[3] = np.array([[0.3, 0.7], [0.2, 0.8]])

	N = 10000
	main(R, P, N, D_len, S_len)