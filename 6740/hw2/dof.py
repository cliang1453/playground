import numpy as np 
import matplotlib.pyplot as plt
from sklearn.tree import DecisionTreeRegressor

mu, sigma = 0, 1 
N = 100
B = 10
F = 10

x = np.random.normal(mu, sigma, [N, F])
y = np.random.normal(mu, sigma, N)
y_hat, y_draw = np.zeros([B, N]), np.zeros([B, N])
S = np.zeros(B)
cov = np.zeros(N)


for num_nodes in [1, 5, 10, N]:
	if num_nodes != 1:
		r = DecisionTreeRegressor(random_state=0, min_samples_leaf=1, max_leaf_nodes=num_nodes)

	for b in range(B):
		idx_b = np.random.randint(low=0, high=N, size=N)
		x_b, y_b = x[idx_b], y[idx_b]
		
		if num_nodes == 1:
			y_hat_b = np.mean(y_b)*np.ones(N)
		else:
			r.fit(x_b, y_b)
			y_hat_b = r.predict(x_b)
		
		y_draw[b, :] = y_b
		y_hat[b, :] = y_hat_b
		
		y_b = np.expand_dims(y_b, axis=0)
		y_hat_b = np.expand_dims(y_hat_b, axis=0)
		S[b] = np.trace(np.dot(np.linalg.pinv(np.dot(y_b.T, y_b)), np.dot(y_b.T, y_hat_b)))
		
	Ey_hat = np.mean(y_hat, axis=0)
	Ey_draw = np.mean(y_draw, axis=0)
	#print(y_hat == y_draw)

	for i in range(N):
		cov[i] = np.mean((y_hat[:, i] - Ey_hat[i])*(y_draw[:, i]-Ey_draw[i]))

	df = np.sum(cov)
	df_s = np.mean(S)*N
	print(df)
	print(df_s)
	print("======")