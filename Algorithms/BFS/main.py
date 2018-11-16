##
# main.py
# Ankit Srivastava <asrivast@gatech.edu>
# Georgia Institute of Technology, Fall 2018
#
# The main entry point for computing HC centrality

import fileio
import hcc


def main(argv):
    if len(argv) != 4:
        raise ValueError("Usage: python %s <input-graph-file> <heist-nodes-file> <output-file>\n" % argv[0])

    gr_file_path, hn_file_path, out_file_path = argv[1:]

    # Open the input and use it to create the graph
    g = fileio.graph_from_gr(gr_file_path)

    # Load the vertex set h
    h = fileio.vertexset_from_file(hn_file_path)

    # Compute HC centrality for each vertex
    # TODO: implement this in hcc.py
    hcc.compute(g, h)

    # Write out the HC centrality to a file
    fileio.write_vdata_to_file(g, out_file_path)


if __name__ == '__main__':
    import sys
    main(sys.argv)
