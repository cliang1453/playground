##CSE6140 Project Travelling Saleman##

#File Structure#
TSP.py -- main run file
branch_bound_bfs_pruning.py -- A breadth-first search based branch&bound algorithm, include as reference
branch_bound_dfs_pruning.py -- A depth-first search based branch&bound algorithm, we use this for final branch&bound submission
hill_climbing_tsp.py -- A hill-climbing local search algorithm
MST_APPROX.py -- An MST approximation algorithm
utils.py -- data loader and data writer
tsp.cc -- A simulated annealing local search algorithm

#Run#
python TSP.py -inst <filename> -alg [BnB | Approx | LS1 | LS2] -time <cutoff_in_seconds> [-seed <random_seed>]
