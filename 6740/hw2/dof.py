import numpy as np 
import matplotlib.pyplot as plt
from sklearn.tree import DecisionTreeRegressor

mu, sigma = 0, 1 
N = 100 
B = 1000
F = 10

x = np.random.normal(mu, sigma, [N, F])
y = np.random.normal(mu, sigma, N)
y_hat, y_draw = np.zeros([B, N]), np.zeros([B, N])
cov = np.zeros(N)


for num_nodes in [1, 5, 10, N]:
	if num_nodes != 1:
		r = DecisionTreeRegressor(random_state=0, min_samples_leaf=1, max_leaf_nodes=num_nodes)

	for b in range(B):
		idx_b = np.random.randint(low=0, high=N, size=N)
		x_b, y_b = x[idx_b], y[idx_b]
		
		if num_nodes == 1:
			y_hat[b, :] = np.mean(y_b)*np.ones(N)
		else:
			r.fit(x_b, y_b)
			y_hat[b, :] = r.predict(x_b)
		y_draw[b, :] = y_b
		
	Ey_hat = np.mean(y_hat, axis=0)
	Ey_draw = np.mean(y_draw, axis=0)

	for i in range(N):
		cov[i] = np.mean((y_hat[:, i] - Ey_hat[i])*(y_draw[:, i]-Ey_draw[i]))

	df = np.sum(cov)
	print(df)