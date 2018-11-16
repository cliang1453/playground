##
# hcc.py
# Chen Liang
# Georgia Institute of Technology, Fall 2018
#
# Compute HC centrality measure

import queue
import time
import numpy as np
'''
        g.n: # of vertices in the graph
        g.m: # edges of in the graph 

        g.offset: a list {index: the index of src vertex (index from 0->n-1), 
                          value: the index of first edge in order starting from src vertex (discrete value from 0->m-1)}
        g.edges: a list {index: the index of edge (index from 0->m-1),
                         value: the index of dst vertex (index from 0->n-1)}
        

        g.degree(vertex_index): return the number of edges starting from the this vertex
        g.vertex_edges(vertex_index): return all the dst vertices from this vertex


        h: set: contains all the indices of heist vertices
'''
def bfs(h_node, g):

	if h_node >= g.n or h_node < 0:
		raise RuntimeError("invalid heist location")

	Q = queue.Queue()
	Q.put(h_node)
	dist = [0 for n in range(g.n)]
	visited = set([h_node])

	while not Q.empty():
		cur_node = Q.get()
		for neighbor in g.vertex_edges(cur_node):
			if neighbor in visited:
				continue
			dist[neighbor] = dist[cur_node] + 1
			visited.add(neighbor)
			Q.put(neighbor)

	return dist


def compute(g, h):
    
    dist = []
    
    for h_v in h:
    	dist.append(bfs(h_v, g))

    sum_ = [sum(x) for x in zip(*dist)]
    g.vdata = [float(1.0/sum(x)) for x in zip(*dist)]