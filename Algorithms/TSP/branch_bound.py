from utils import data_loader, data_writer
import numpy as np
import sys
import math
import os
import time

class Node(object):
	"""docstring for ClassName"""
	def __init__(self, parent_matrix=None, parent_cost=None, edge_to_explore=None):
		
		self.matrix = parent_matrix.copy()
		self.parent_cost = parent_cost
		self.edge_to_explore = edge_to_explore

		if self.edge_to_explore is not None:
			self.s = edge_to_explore[0]
			self.d = edge_to_explore[1]
			self.edge_cost = self.matrix[self.s][self.d]
		else:
			self.s, self.d = None, None
			self.edge_cost = 0


	def reduce(self):
		if self.edge_cost != 0:
			self.matrix[self.s, :] = math.inf
			self.matrix[:, self.d] = math.inf
			self.matrix[self.d][self.s] = math.inf
		
		# search row for smallest value
		min_row = self.matrix.min(axis=1)
		min_row[min_row==math.inf] = 0
		self.matrix -= min_row
		min_col = self.matrix.min(axis=0)
		min_col[min_col==math.inf] = 0
		self.matrix -= min_col

		return np.sum(min_row) + np.sum(min_col)

	def get_total_cost(self):
		return self.parent_cost + self.edge_cost + self.reduce()


class SearchTree(object):
	"""docstring for ClassName"""
	def __init__(self, args, matrix, V):
		self.args = args
		self.result = (math.inf, [])
		self.matrix = matrix
		self.V = V
		

	def search(self, parent, parent_cost, parent_v, result_list, result_dist, visited):

		if len(result_list) == self.V:
			result_dist += self.matrix[parent_v][0]
			if result_dist < self.result[0]:
				self.result = (result_dist, result_list.copy())
				data_writer(result_dist=result_dist, \
							time=time.time() - self.args[0],\
							instance=os.path.splitext(os.path.basename(self.args[2]))[0], \
							cutoff=self.args[1])
			return

		min_cost = math.inf
		parent_next_cand = {}
		
		for v in range(self.V):
			
			if v in visited:
				continue
			curr = Node(parent_matrix=parent.matrix, parent_cost=parent_cost, edge_to_explore=(parent_v, v))
			curr_cost = curr.get_total_cost()
			
			if curr_cost <= min_cost:
				min_cost = curr_cost
				if min_cost not in parent_next_cand:
					parent_next_cand[min_cost] = []
				parent_next_cand[min_cost].append(curr)
		
		
		for parent_next in parent_next_cand[min_cost]:

			s, d = parent_next.edge_to_explore
			result_list.append(d)
			visited.add(d)
			result_dist += self.matrix[s][d]
			
			self.search(parent_next, min_cost, d, result_list, result_dist, visited)
			if time.time() - self.args[0] > self.args[1]:
				return
			
			result_list.pop()
			visited.remove(d)
			result_dist -= self.matrix[s][d]

	
		


def main(argv): 
	
	if len(argv) != 3:
		raise ValueError("Usage: python %s <input-dataset-name> <cut-off time(s))>\n" % argv[0])
	dataset_path, cutoff_time = argv[1:]
	
	matrix, V = data_loader(dataset_path)
	parent = Node(parent_matrix=matrix, parent_cost=0)
	parent_cost = parent.get_total_cost()

	tree = SearchTree(args=(time.time(), int(cutoff_time), dataset_path), \
					  matrix=matrix, V=V)
	
	tree.search(parent=parent, parent_cost=parent_cost, parent_v=0, \
		   		result_list=[0], result_dist=0, visited=set([0]))
	
	data_writer(result_list=tree.result[1], \
			    result_dist=tree.result[0], \
		        instance=os.path.splitext(os.path.basename(dataset_path))[0], \
		        cutoff=cutoff_time)


if __name__ == '__main__':
	main(sys.argv)