import numpy as np

def main():

	for D in ["1111","1112","1122","1121","1221","1222","1212","1211",\
			  "2211","2212","2222","2221","2121","2122","2112","2111"]:
		
		d = np.array([int(D[0]), int(D[1]), int(D[2]), int(D[3])])
		P = np.array([[0.3, 0.4, 0.2, 0.1], [0.2, 0.3, 0.5, 0.0], [0.1, 0.0, 0.8, 0.1], [0.4, 0.0, 0.0, 0.6]])
		p_a1 = np.array([1, 2, 3, 4])
		p_a2 = 20
		d_a1 = np.array(d==1)
		d_a2 = np.array(d==2)
		r_d = p_a1 * d_a1 + p_a2 * d_a2
		P_d = (d_a1 * P.T).T
		I = np.eye(4)
		V = np.matmul(np.linalg.inv(I - 0.9 * P_d), r_d)
		print(V)

if __name__ == '__main__':
	main()