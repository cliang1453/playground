import argparse
import numpy as np
import os
import math
from greedy import data_loader


def ridge (X, y, Lambda):	
	beta = np.dot(np.linalg.inv(np.matmul(X.T,X) + 2000*Lambda*np.eye(X.shape[1])), np.matmul(X.T, y))
	return beta


def get_error(X, y, beta, Lambda, true_beta = None):
	
	pred_error = np.power(np.linalg.norm(y - np.dot(X, beta)), 2)/X.shape[0]
	
	beta_error = 0
	if true_beta is not None:
		beta_error = np.power(np.linalg.norm(beta - true_beta), 2)
	
	return pred_error, beta_error

def ten_fold (X, y, Lambdas = [0.0125, 0.025, 0.05, 0.1, 0.2]):

	gap = int(len(X)/10)
	error = []

	for i in range(10):
		
		cur_error = []
		train_data = np.concatenate((X[0:i*gap],X[(i+1)*gap:]), axis=0)
		val_data = X[i*gap: (i+1)*gap]
		train_label = np.concatenate((y[0:i*gap],y[(i+1)*gap:]), axis=0)
		val_label = y[i*gap: (i+1)*gap]

		train_data = np.concatenate((train_data, np.ones([train_data.shape[0], 1])), axis=1)

		for lamda in Lambdas:
			
			beta = ridge(train_data, train_label, lamda)
			pred_error, _ = get_error(X = val_data, y = val_label, beta = beta[:-1], Lambda = lamda)
			cur_error.append(pred_error)
		
		error.append(cur_error)

	idx = np.argmin(np.mean(np.array(error), axis=0))
	Lambda = Lambdas[idx]

	X = np.concatenate((X, np.ones([X.shape[0], 1])), axis=1)
	beta = ridge(X, y, Lambda)

	return beta[:-1], Lambda


def main(args):

	data, label = data_loader(args.train_file)
	test_data, test_label = data_loader(args.test_file)
	true_beta, _ = data_loader(args.beta_file, load_beta = True)

	beta, Lambda = ten_fold(data, label) 
	pred_error, beta_error = get_error(X = test_data, y = test_label, 
		beta = beta, Lambda = Lambda, true_beta = true_beta)
	

if __name__ == "__main__":

	parser = argparse.ArgumentParser(description='parse cmd arguments')

	parser.add_argument('--train_file', type=str, 
						default='train-matrix.txt',
						help='train file')
	parser.add_argument('--beta_file', type=str, 
						default='true-beta.txt',
						help='beta file')
	parser.add_argument('--test_file', type=str, 
						default='test-matrix.txt',
						help='test file')
	
	args = parser.parse_args()
	main(args)