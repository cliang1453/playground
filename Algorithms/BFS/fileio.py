##
# fileio.py
# Ankit Srivastava <asrivast@gatech.edu>
# Georgia Institute of Technology, Fall 2018
#
# Helper functions for reading graphs and writing data


from graph import Graph, VertexSet


def graph_from_gr(gr_file):
    """
    Create a new graph object from a simple dimacs-like file
    """
    # Open the file
    with open(gr_file, 'rb') as gf:
        # Read the number of vertices and edges
        n, m = [int(l) for l in gf.readline().split()]

        # n: # of vertices
        # m: # of edges 

        # Create a new graph with the appropriate size
        g = Graph(n, m)
        # g.offsets: [None .. None] length = n
        # g.edges: [None .. None] length = m

        # Read through the file parsing edges, assuming that they are in order
        cur = n
        for offset in range(m):
            src, dst = [int(l) for l in gf.readline().split()]
            while cur != src:
                if cur == n:
                    cur = 0
                else:
                    cur += 1
                if cur > src:
                    raise RuntimeError("invalid edge")
                g.offsets[cur] = offset
            # Add the edge in
            g.edges[offset] = dst

        # Finish the final edges
        for final in range(cur+1, n):
            g.offsets[final] = m
        return g

        '''
        g.offset: a list {index: the index of src vertex (index from 0->n-1), 
                          value: the index of edge starting from src vertex (index from 0->m-1)}
        g.edges: a list {index: the index of edge starting from src vertex (index from 0->m-1),
                         value: the index of dst vertex (index from 0->n-1)}
        '''


def vertexset_from_file(vertex_file):
    """
    Create a vertex set object from a simple file with vids
    """
    # Open the file
    with open(vertex_file, 'rb') as vf:
        # Read the number of elements in the set
        k = int(vf.readline())
        # Read the remaining lines and read/insert each into a new VertexSet
        vs = VertexSet()
        for v in vf.readlines():
            vs.add(int(v))
            
        assert len(vs) == k
        return vs


def write_vdata_to_file(g, vdata_file):
    """
    Save vertex data in a graph to the given file
    """
    with open(vdata_file, 'w') as out:
        out.write('%d\n' % g.n)
        for vd in g.vdata:
            out.write('%f\n' % vd)
