import numpy as np 
import matplotlib.pyplot as plt
from sklearn.tree import DecisionTreeRegressor

mu, sigma = 0, 1 # mean and standard deviation
num_samples = 100
num_features = 10
num_iter = 10

x = np.random.normal(mu, sigma, [num_samples, num_features])
y = np.random.normal(mu, sigma, num_samples)
y_hat_5, y_hat_10 = np.zeros([num_samples, num_iter]), np.zeros([num_samples, num_iter])
E_y = np.mean(y)


r_5 = DecisionTreeRegressor(random_state=0, max_leaf_nodes=5)
r_10 = DecisionTreeRegressor(random_state=0, max_leaf_nodes=10)

for i in range(num_iter):	
	r_5.fit(x, y)
	y_hat_5[:, i] = r_5.predict(x)
	r_10.fit(x, y)
	y_hat_10[:, i] = r_10.predict(x)

y_hat_1 = np.ones([num_samples]) * np.mean(y)
y_hat_5 = np.mean(y_hat_5, axis=1)
y_hat_10 = np.mean(y_hat_10, axis=1)
E_y_hat_1 = np.ones([num_samples]) * np.mean(y_hat_1)
E_y_hat_5 = np.ones([num_samples]) * np.mean(y_hat_5)
E_y_hat_10 = np.ones([num_samples]) * np.mean(y_hat_10)
print(y_hat_1.shape)
print(y_hat_5.shape)
print(y_hat_10.shape)
c = np.sum((y-E_y)*(y_hat_1- E_y_hat_1))/num_samples
a = np.sum((y-E_y)*(y_hat_5- E_y_hat_5))/num_samples
b = np.sum((y-E_y)*(y_hat_10- E_y_hat_10))/num_samples
print(c)
print(a)
print(b)