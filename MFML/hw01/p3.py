import matplotlib
matplotlib.use('Agg')
import numpy as np
import matplotlib.pyplot as plt

def polynomial_spline(data_t, data_y):
	T = np.array([	[0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
					[1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
					[0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0],
					[0, 0, 0, 0, 8, 4, 2, 1, 0, 0, 0, 0],
					[0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 2, 1],
					[0, 0, 0, 0, 0, 0, 0, 0, 27, 9, 3, 1],
					[3, 2, 1, 0, -3, -2, -1, 0, 0, 0, 0, 0],
					[0, 0, 0, 0, 12, 4, 1, 0, -12, -4, -1, 0],
					[6, 2, 0, 0, -6, -2, 0, 0, 0, 0, 0, 0],
					[0, 0, 0, 0, 12, 2, 0, 0, -12, -2, 0, 0],
					[0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
					[0, 0, 0, 0, 0, 0, 0, 0, 18, 2, 0, 0 ]])
	y = np.array([3.6, 2.75, 2.75, -1.35, -1.35, 3.0, 0, 0, 0, 0, 0, 0])
	alpha = np.linalg.inv(T).dot(y)
	a0, b0, c0, d0 = alpha[0], alpha[1], alpha[2], alpha[3]
	a1, b1, c1, d1 = alpha[4], alpha[5], alpha[6], alpha[7]
	a2, b2, c2, d2 = alpha[8], alpha[9], alpha[10], alpha[11]
	print((a0, b0, c0, d0))
	print((a1, b1, c1, d1))
	print((a2, b2, c2, d2))
	t0 = np.linspace(0,1,100)
	Y0 = a0 * np.power(t0,3) + b0 * t0**2 + c0 * t0 + d0
	t1 = np.linspace(1,2,100)
	Y1 = a1 * np.power(t1,3) + b1 * t1**2 + c1 * t1 + d1
	t2 = np.linspace(2,3,100)
	Y2 = a2 * np.power(t2,3) + b2 * t2**2 + c2 * t2 + d2
	plt.plot(t0, Y0, t1, Y1, t2, Y2)
	plt.scatter(data_t, data_y)
	plt.savefig('p3b.png')


def cubic_polynomial(data_t, data_y):
	T = np.array([[0, 0, 0, 1],\
				  [1, 1, 1, 1],\
				  [8, 4, 2, 1],\
				  [27,9, 3, 1]])
	y = data_y.copy()
	alpha = np.linalg.inv(T).dot(y)
	a, b, c, d = alpha[0], alpha[1], alpha[2], alpha[3]
	print((a, b, c, d))
	t = np.linspace(0,3,300)
	Y = a * np.power(t,3) + b * t**2 + c * t + d
	plt.plot(t, Y)
	plt.scatter(data_t, data_y)
	plt.savefig('p3a.png')


if __name__ == '__main__':

	data_t = np.array([0,1,2,3])
	data_y = np.array([3.6, 2.75, -1.35, 3.0])
	cubic_polynomial(data_t, data_y)
	polynomial_spline(data_t, data_y)