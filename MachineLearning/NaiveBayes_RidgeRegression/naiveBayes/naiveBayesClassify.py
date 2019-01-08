import argparse
import numpy as np
import os
import math
import scipy
import scipy.io
from prior import data_loader, prior
from likelihood import likelihood
import matplotlib.pyplot as plt


def naiveBayesClassify(xTest, M, V, p):
	'''
	Input: 
	xTest: [num_samples, hidden_dim] --> [num_samples, hidden_dim, num_classes]
	M: [hidden_dim, num_classes]
	V: [hidden_dim, num_classes]
	p: [num_classes, 1] --> [1, num_classes]

	output: 
	nb: [num_samples, 1]
	'''

	# construct posterior dist
	x = np.repeat(np.expand_dims(xTest, axis=2), p.shape[0], axis=2) # [num_samples, hidden_dim, num_classes]
	post = np.log(1/np.power(2*np.pi*V, 0.5)*np.exp(-np.power(x - M, 2)/(2*V))) # [num_samples, hidden_dim, num_classes]

	# log p(c|f) = log p(c) + log p(f1|c) + log p(f2|c) + ....
	score = np.log(p.T) + np.sum(post, axis=1) # [num_samples, num_classes]
	nb = np.expand_dims(np.argmax(score, axis=1) + 1, axis=1) # [num_samples, 1]

	return nb

def eval(nb, yTest, file):
	
	nb = np.squeeze(nb, axis=1)
	yTest = np.squeeze(yTest, axis=1)
	with open(file, 'w+') as f:
		f.write("%f\n" % (1 - np.count_nonzero(nb - yTest)/nb.shape[0]))
		f.write("%f\n" % (np.count_nonzero(yTest[nb==1]==1)/np.count_nonzero(nb == 1)))
		f.write("%f\n" % (np.count_nonzero(nb[yTest==1]==1)/np.count_nonzero(yTest == 1)))
		f.write("%f\n" % (np.count_nonzero(yTest[nb==5]==5)/np.count_nonzero(nb == 5)))
		f.write("%f" % (np.count_nonzero(nb[yTest==5]==5)/np.count_nonzero(yTest == 5)))


def main(args):
	
	xTrain, yTrain, xTest, yTest = data_loader(args.data_file)
	p = prior(yTrain)
	M, V = likelihood(xTrain, yTrain)
	nb = naiveBayesClassify(xTest, M, V, p)
	eval(nb, yTest, args.output_file)



if __name__ == "__main__":
	parser = argparse.ArgumentParser(description='parse cmd arguments')
	parser.add_argument('--data_file', type=str, 
						default='ecoli.mat',
						help='data file')
	parser.add_argument('--output_file', type=str, 
						default='evaluation.txt',
						help='output file')
	args = parser.parse_args()
	main(args)