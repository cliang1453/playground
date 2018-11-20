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

		if len(self.path) >= 2:
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
		self.matrix -= min_row
		min_col = self.matrix.min(axis=0)
		min_col[min_col==math.inf] = 0
		self.matrix -= min_col

		return np.sum(min_row) + np.sum(min_col)

	def get_total_cost(self):
		return self.parent_cost + self.edge_cost + self.reduce_sum

	def __lt__(self, other):
		return self.get_total_cost() < other.get_total_cost()

	

def get_path_sum(path, matrix):

	path_sum = matrix[path[-1]][path[0]]
	for v in range(len(path)-1):
		path_sum += matrix[path[v]][path[v+1]]
		
	return path_sum


def main(argv): 
	
	if len(argv) != 3:
		raise ValueError("Usage: python %s <input-dataset-name> <cut-off time(s))>\n" % argv[0])
	dataset_path, cutoff_time = argv[1:]
	cutoff_time = int(cutoff_time)

	matrix, V = data_loader(dataset_path)
	live = []
	heapq.heappush(live, Node(matrix, 0, 0, []))
	start = time.time()
	min_sum = math.inf

	while live:
		
		curr = heapq.heappop(live)
		# print(curr.visited)
		# print(curr.get_total_cost())

		if len(curr.path)==V:
			curr_sum = get_path_sum(curr.path, matrix)
			if curr_sum < min_sum:
				min_sum = curr_sum
				min_path = curr.path

				data_writer(result_dist=min_sum, \
							time=time.time() - start,\
							instance=os.path.splitext(os.path.basename(dataset_path))[0], \
							cutoff=cutoff_time)
			
		if time.time() - start > cutoff_time:
			
			curr_path = [curr.path.copy()]
			for v in range(V):
				if v in curr.path:
					continue
				else:
					curr_path.append(v)
			curr_sum = get_path_sum(curr_path, matrix)

			data_writer(result_list=curr_path , \
						result_dist=curr_sum + , \
						instance=os.path.splitext(os.path.basename(dataset_path))[0], \
						cutoff=cutoff_time)
			return 

		
		for v in range(V):
			if v in curr.visited:
				continue
			heapq.heappush(live, Node(curr.matrix, curr.get_total_cost(), v, curr.path))


	data_writer(result_list=min_path, \
				result_dist=min_sum, \
				instance=os.path.splitext(os.path.basename(dataset_path))[0], \
				cutoff=cutoff_time)


if __name__ == "__main__":
	main(sys.argv)