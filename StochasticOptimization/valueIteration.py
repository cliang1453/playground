import numpy as np

def main():


	S = [0, 1, 2]

	# r_1[x] = r(1, a_(1, x))
	r_1 = np.array([0, 0])
	r_2 = np.array([3, 4])
	r_3 = np.array([4])

	# p_11[j][x] = p(j | 1, a_(1, x))
	p_11 = np.array([0.5, 0.5, 0])
	# p_12[j] = p(j | 1, a_(1, 2))
	p_12 = np.array([2.0/3.0, 0, 1.0/3.0])
	# p_21[j] = p(j | 2, a_(2, 1))
	p_21 = np.array([1, 0, 0])
	# p_22[j] = p(j | 2, a_(2, 2))
	p_22 = np.array([0, 0, 1])
	# p_31[j] = p(j | 3, a_(3, 1))
	p_31 = np.array([1, 0, 0])

	# initialization
	epsilon = 1e-3
	lamda = 0.8
	V = np.zeros(3)
	n = 0

	while True:
		
		V_1 = r_1 + np.array([np.sum(p_11 * V * lamda), np.sum(p_12 * V * lamda)])
		V_2 = r_2 + np.array([np.sum(p_21 * V * lamda), np.sum(p_22 * V * lamda)])
		V_3 = r_3 + np.array([np.sum(p_31 * V * lamda)])

		V_new = np.array([np.max(V_1), np.max(V_2), np.max(V_3)])
		
		err = np.linalg.norm(V_new - V)
		if err < epsilon * (1 - lamda) / (2 * lamda):
			break
		else:
			V = V_new

		print(V)
		print(err)
	
	D = np.array([np.argmax(V_1), np.argmax(V_2), np.argmax(V_3)])
	print(D + 1)

if __name__ == '__main__':
	main()