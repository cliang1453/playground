import numpy as np 
import matplotlib.pyplot as plt
from sklearn.tree import DecisionTreeRegressor

mu, sigma = 0, 1 # mean and standard deviation
y = np.random.normal(mu, sigma, 100)
u = np.random.normal(mu, sigma, 100)
E_y = np.mean(y)
E_u = np.mean(u)

r_1 = DecisionTreeRegressor(random_state=0, max_leaf_nodes=1)
r_5 = DecisionTreeRegressor(random_state=0, max_leaf_nodes=5)
r_10 = DecisionTreeRegressor(random_state=0, max_leaf_nodes=10)
fit(X, y, sample_weight=None, check_input=True, X_idx_sorted=None)
