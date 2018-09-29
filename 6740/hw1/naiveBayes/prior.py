import argparse
import numpy as np
import os
import math
import scipy
import scipy.io

def prior(yTrain):
	
	yTrain = np.squeeze(yTrain)
	p = np.zeros(np.max(yTrain))
	
	for i in yTrain:
		p[i-1] += 1
	
	p = np.expand_dims(p/yTrain.shape[0], axis=1)
	
	return p


def data_loader(file):
	data_mat = scipy.io.loadmat('ecoli.mat')
	return data_mat['xTrain'], data_mat['yTrain'], data_mat['xTest'], data_mat['yTest']

def main(args):
	_, yTrain, _, _ = data_loader(args.data_file)
	p = prior(yTrain)

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description='parse cmd arguments')
	parser.add_argument('--data_file', type=str, 
						default='ecoli.mat',
						help='data file')
	args = parser.parse_args()
	main(args)