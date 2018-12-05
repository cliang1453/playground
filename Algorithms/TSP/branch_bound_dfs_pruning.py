from utils import data_loader, data_writer
import numpy as np
import sys
import math
import os
import time
import heapq

class Node(object):
	"""docstring for ClassName"""
	def __init__(self, parent_matrix, parent_cost, v, parent_path):

		self.path = parent_path.copy()
		if v is not None:
			self.path.append(v)

		self.visited = set(self.path)
		self.matrix = parent_matrix.copy()
		self.parent_cost = parent_cost

		if len(self.path) >= 2 and v is not None:
			self.s = self.path[len(self.path)-2]
			self.d = self.path[-1]
			self.edge_cost = self.matrix[self.s][self.d]
		else:
			self.s, self.d = None, None
			self.edge_cost = 0

		self.reduce_sum = self.reduce()


	def reduce(self):
		
		if self.edge_cost != 0:
			self.matrix[self.s, :] = math.inf
			self.matrix[:, self.d] = math.inf
			self.matrix[self.d][self.s] = math.inf
		
		# search row for smallest value
		min_row = self.matrix.min(axis=1)
		min_row[min_row==math.inf] = 0
		self.matrix = (self.matrix.T - min_row).T
		min_col = self.matrix.min(axis=0)
		min_col[min_col==math.inf] = 0
		self.matrix -= min_col

		return np.sum(min_row) + np.sum(min_col)

	def get_total_cost(self):
		return self.parent_cost + self.edge_cost + self.reduce_sum

	def __lt__(self, other):
		return self.get_total_cost() < other.get_total_cost()


class searchTree(object):
	
	def __init__(self, args, matrix, V):
		self.V = V
		self.matrix = matrix
		self.min_dist = math.inf
		self.min_path = []
		self.args = args

	def get_path_sum(self, path):
		
		path_sum = self.matrix[path[-1]][path[0]]
		for v in range(len(path)-1):
			path_sum += self.matrix[path[v]][path[v+1]]
			
		return path_sum

	def check_update(self, curr):

		curr_dist = self.get_path_sum(curr.path)
		
		if curr_dist < self.min_dist:
			
			self.min_dist = curr_dist
			self.min_path = curr.path
			
			data_writer(result_dist=self.min_dist, \
						time=time.time() - self.args[0],\
						instance=os.path.splitext(os.path.basename(self.args[2]))[0], \
						cutoff=self.args[1],\
						save='RESULTS_DFS/')

	def binary_lb(self, curr, v):

		cand1 = Node(curr.matrix, curr.get_total_cost(), v, curr.path)
		lb1 = cand1.get_total_cost()

		m = curr.matrix.copy()
		m[curr.path[-1], v] = math.inf
		cand2 = Node(m, curr.get_total_cost(), None, curr.path)
		lb2 = cand2.get_total_cost()

		return cand1, cand2, lb1, lb2

	def search(self, curr):

		if len(curr.path)==self.V:
			self.check_update(curr)

		if time.time() - self.args[0] > self.args[1]:
			return 

		n_cand1, n_cand2 = None, None
		lb_low = [self.min_dist, self.min_dist]
		
		for v in range(self.V):
			
			if v in curr.visited:
				continue

			cand1, cand2, lb1, lb2 = self.binary_lb(curr, v)
			if lb1 == math.inf: 
				# it means this edge has been eliminated
				# and thus the rest has been added to the queue already 
				continue

			if lb1 < lb_low[0]:
				lb_low[0] = lb1
				lb_low[1] = lb2
				n_cand1 = cand1
				n_cand2 = cand2

		
							
		if lb_low[0] < self.min_dist:
			self.search(n_cand1)
		if lb_low[1] < self.min_dist:
			self.search(n_cand2)
		return 


def main(argv): 
	
	if len(argv) != 3:
		raise ValueError("Usage: python %s <input-dataset-name> <cut-off time(s))>\n" % argv[0])
	dataset_path, cutoff_time = argv[1:]
	cutoff_time = int(cutoff_time)

	matrix, V = data_loader(dataset_path)
	start = time.time()
	search_tree = searchTree((start, cutoff_time, dataset_path), matrix, V)
	root = Node(matrix, 0, 0, [])
	search_tree.search(root)

	data_writer(result_list=search_tree.min_path, \
				result_dist=search_tree.min_dist, \
				instance=os.path.splitext(os.path.basename(dataset_path))[0], \
				cutoff=cutoff_time,\
				save='RESULTS_DFS/')


if __name__ == "__main__":
	main(sys.argv)