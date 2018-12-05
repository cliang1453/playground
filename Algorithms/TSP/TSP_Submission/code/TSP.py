# main file
import sys
from hill_climbing_tsp import run_hill_climb
from branch_bound_dfs_pruning import run_bnb
from MST_APPROX import run_mst
from ctypes import *
import os



def main(argv):

    '''
    exec -inst <filename>
         -alg [BnB | Approx | LS1 | LS2]
         -time <cutoff_in_seconds>
         [-seed <random_seed>]
    '''
    
    if len(argv) < 4 or len(argv) > 5:
        raise ValueError("Usage: python %s <inst <filename>> <alg [BnB | Approx | LS1 | LS2]> <time <cutoff_in_seconds>> <[seed <random_seed>]>\n" % argv[0])

    if len(argv) == 4:
        filename, method, cutoff_time = argv[1:]
    else:
        filename, method, cutoff_time, seed = argv[1:]
    cutoff_time = int(cutoff_time)

    if method == 'BnB':
        run_bnb(filename, cutoff_time)
    elif method == 'LS1':
        run_hill_climb(filename, cutoff_time, seed)
    elif method == 'LS2':
        os.system('g++ -Wall -g -O2 -std=c++17 tsp.cc -fPIC -shared -o tsp.so')
        tsp = cdll.LoadLibrary('./tsp.so')
        tsp.f.argtypes = [c_char_p]*5
        tsp.f.restype = c_int
        loc = os.path.basename(filename).split('.')[0]
        tsp.f(filename.encode(), (loc + "_LS2_" + str(cutoff_time) + "_" + str(seed) + ".sol").encode() ,\
            (loc + "_LS2_" + str(cutoff_time) + "_" + str(seed) + ".trace").encode() , (str(cutoff_time)).encode(), str(seed).encode())
    elif method =='Approx':
        run_mst(filename, cutoff_time)
    else:
        raise ValueError("Invalid method")


if __name__ == "__main__":
	main(sys.argv)