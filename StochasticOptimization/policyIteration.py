import numpy as np
import itertools

def policy_iteration(P = None, R = None, S = None, A = None, lamda = 0.9):

	n = 0
	d = [1, 0, 1, 0, 0]
	I = np.eye(len(S))
	P_d = np.zeros((len(S), len(S)))
	R_d = np.zeros(len(S))
	P_new_d = np.zeros((len(S), len(S)))
	R_new_d = np.zeros(len(S))
	
	while True:

		for s in S:
			P_d[s, :] = P[s, :, d[s]]
			R_d[s] = R[s, d[s]]
		
		# policy evaluation
		V_d = np.matmul(np.linalg.inv( I - lamda * P_d ), R_d)

		print((d, V_d))

		# policy improvement
		update_d = False
		V_new_d = V_d.copy()

		for new_d in D: 

			for s in S:
				P_new_d[s, :] = P[s, :, new_d[s]]
				R_new_d[s] = R[s, new_d[s]]

			if np.sum(R_new_d + lamda * np.matmul(P_new_d, V_d)) > np.sum(V_new_d):
				V_new_d = R_new_d + lamda * np.matmul(P_new_d, V_d)
				d = new_d.copy()
				update_d = True

		if not update_d:
			break 

	print((d, V_d))


if __name__ == '__main__':

	S = [0, 1, 2, 3, 4]
	A = [0, 1]

	# P[s][j][a] = P(j | s, a)
	P = np.zeros((len(S), len(S), len(A)))
	P[:4,:4,0] = np.array([[0.3, 0.4, 0.2, 0.1],[0.2, 0.3, 0.5, 0.0],[0.1, 0.0, 0.8, 0.1],[0.4, 0.0, 0.0, 0.6]])
	P[ 4, 4, 0] = 1
	P[:4, 4, 1] = 1

	# R[s][a] = r(s, a) 
	R = np.zeros((len(S), len(A)))
	R[:4, 0] = np.array([1, 2, 3, 4])
	R[:4, 1] = 20

	# decision space
	D = [list(i) + [0] for i in itertools.product([0, 1], repeat=4)]
	lamda = 0.9

	policy_iteration(P, R, S, A, lamda)