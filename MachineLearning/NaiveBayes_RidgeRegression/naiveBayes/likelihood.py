import argparse
import numpy as np
import os
import math
import scipy
import scipy.io
from prior import data_loader

def likelihood (xTrain, yTrain):
	
	"""
	input: 
		XTrain: data [num_samples, hidden_dim]
		yTrain: label [num_samples, 1]
	output: 
		M: [hidden_dim, num_class]
		V: [hidden_dim, num_class]
	"""

	yTrain = np.squeeze(yTrain)
	M, V = [], []

	for i in range(np.max(yTrain)):
		feature_dist_class_i = xTrain[(yTrain == i+1)]
		feature_mean_class_i = np.mean(feature_dist_class_i, axis=0)
		feature_var_class_i = np.var(feature_dist_class_i, axis=0)
		M.append(feature_mean_class_i)
		V.append(feature_var_class_i)

	M = np.stack( M, axis=0 ).T
	V = np.stack( V, axis=0 ).T
	return M, V


def main(args):
	xTrain, yTrain, _, _ = data_loader(args.data_file)
	M, V = likelihood(xTrain, yTrain)

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description='parse cmd arguments')
	parser.add_argument('--data_file', type=str, 
						default='ecoli.mat',
						help='data file')
	args = parser.parse_args()
	main(args)