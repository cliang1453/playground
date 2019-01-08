import argparse
import numpy as np
import os
import math

def greedy(X = None, y = None, K = 6):
	"""
	input: 
		X: data [num_samples, hidden_dim] -> [num_samples, hidden_dim + 1]
		y: label [num_samples, ]
		K: # rounds
	output: 
		A: 
		Beta: [hidden_dim, ]
	"""
	
	X = np.concatenate((X, np.ones([X.shape[0], 1])), axis=1)
	A = set([])
	beta = np.zeros(X.shape[1])
	
	for k in range(K):

		i = np.argmax(np.abs(np.matmul(X.T, np.matmul(X, beta) - y)))
		A.add(i)

		beta = np.dot(np.linalg.inv(np.matmul(X.T,X)), np.matmul(X.T, y))
		for i in range(len(beta)-1):
			if i not in A:
				beta[i] = 0

	return A, beta[:-1]


def data_loader(file, load_beta = False):
	with open(file) as f:
		
		lines = f.readlines()
		data, label = [], []

		if load_beta:
			for line in lines[1:1+int(lines[0])]:
				data.append(float(line))
		else:
			for line in lines[2:2+int(lines[0])]:
				data.append([float(i) for i in line.split(' ')])
			for line in lines[2+int(lines[0]):]:
				label.append(float(line))

	return np.array(data), np.array(label)


def main(args):
	train_data, train_label = data_loader(args.train_file)
	A, beta = greedy(X = train_data, y = train_label)


if __name__ == "__main__":
	
	parser = argparse.ArgumentParser(description='parse cmd arguments')
	parser.add_argument('--train_file', type=str, 
						default='train-matrix.txt',
						help='train file')
	
	args = parser.parse_args()
	main(args)