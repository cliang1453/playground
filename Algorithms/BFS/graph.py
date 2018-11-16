##
# graph.py
# Ankit Srivastava <asrivast@gatech.edu>
# Georgia Institute of Technology, Fall 2018
#
# Simple CSR based graph object


from itertools import islice


class Graph(object):
    def __init__(self, n, m):
        # Keep track of the graph size
        self.n = n
        self.m = m

        # Store the graph as edges and offsets into edges
        self.edges = [None for e in range(m)]
        self.offsets = [None for v in range(n)]

        # Include a list for vertex data
        self.vdata = [-1 for v in range(n)]

    def degree(self, v):
        # Degree is either: the difference between this and the next element's pointer,
        # or the difference between the last element and the total number of edges
        if v == self.n - 1:
            return self.m - self.offsets[v]
        else:
            return self.offsets[v+1] - self.offsets[v]

    def vertex_edges(self, v):
        return islice(self.edges, self.offsets[v], self.offsets[v] + self.degree(v))

    def set_vdata(self, v, d):
        self.vdata[v] = d

    def __str__(self):
        s = "Graph: n=%d m=%d\n" % (self.n, self.m)
        s += "Offsets: %s\n" % " ".join(str(o) for o in self.offsets)
        s += "Edges: %s\n" % " ".join(str(e) for e in self.edges)
        s += "Structure: \n"
        for v in range(self.n):
            s += "%d->%s\n" % (v, " ".join(str(v) for v in self.vertex_edges(v)))
        return s


class VertexSet(set):
    def __str__(self):
        s = "VertexSet: k=%d\n" % len(self)
        s += " ".join(str(v) for v in self)
        return s
