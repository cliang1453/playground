# Hill Climbing Iterative Search

import sys
import numpy as np
import networkx as nx
import copy
import time
import random
import math
import os

def create_graph(filename):
    # creates graph from file
    G = nx.Graph()
    # read lines in file
    read = 1
    with open(filename, "r") as f:
        while (read == 1):
            line = f.readline()
            line = line.rstrip()
            if line.startswith('EDGE_WEIGHT_TYPE: '):
                edge_weight_type = line[18:]
            elif line.startswith('NODE_COORD_SECTION'):
                read = 0
                for l in f:
                    if l.startswith('EOF'):
                        break
                    else:
                        # splits each node data into node index, x coordinate, and y coordinate
                        data = list(map(lambda x: float(x), l.split()))
                        G.add_node(data[0], x=data[1], y=data[2])
    f.close()
    # add the edges (need edge between every pair of nodes)
    for u in G.nodes():
        for v in G.nodes():
            if u != v:
                # if the edge weight type is euclidean, we set the weights to be the euclidean distance
                if edge_weight_type == 'EUC_2D':
                    xd = G.node[u]['x'] - G.node[v]['x']
                    yd = G.node[u]['y'] - G.node[v]['y']
                    w = (round((math.sqrt(xd *xd + yd * yd))))
                    G.add_edge(u, v, weight= w)
                # GEO edge weights
                elif edge_weight_type == 'GEO':
                    lat_u, long_u = lat_long(G, u)
                    lat_v, long_v = lat_long(G, v)
                    q1 = math.cos(long_u - long_v)
                    q2 = math.cos(lat_u - lat_v)
                    q3 = math.cos(lat_u + lat_v)
                    w = int((6378.388 * math.acos(.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0))
                    G.add_edge(u, v, weight= w)
    return G


def lat_long(G, curr_node):
    # Finds the longitude and latitude for a given node; used to help with GEO distance
    deg = int(G.node[curr_node]['x'])
    min = G.node[curr_node]['x'] - deg
    lat = np.pi * (deg + 5.0 * min / 3.0) / 180.0
    deg = (int)(G.node[curr_node]['y'])
    min = G.node[curr_node]['y'] - deg
    long = np.pi * (deg + 5.0 * min / 3.0) / 180.0
    return lat, long

def pairings_twoxc(G):
    # All node pairings for 2-opt exchange
    pairings = []
    n = len(G.nodes()) - 1
    for i in range(1,n):
        for j in range(1,n):
            if i < j:
                pairings.append((i, j))
    return pairings


def get_succ(G, pairings, current):
    # given a solution, returns neighborhood of the solution (all the variations of the route) given by reversing the last part of a route
    successors = []
    n = len(G.nodes())
    for (i,j) in pairings:
        if i < j:
            new_route = []
            new_route[0:i] = copy.deepcopy(current[0:i])
            new_route[i:j] = copy.deepcopy(current[i:j])
            new_route[j:n] = list(reversed(current[j:n]))
            successors.append(new_route)
    return successors


def total_cost(solution, G):
    # Finds the cost of a given solution
    visited = 0
    cost = 0
    for node in solution:
        if visited == 0:
            visited = 1
            i = node
        else:
            cost += G[node][i]['weight']
            i = node
    cost += G[solution[0]][solution[-1]]['weight'] # creates a cycle
    return cost

def next_solution(G, current, pairings):
    # Finds the best solution in the neighborhood of a given solution. Uses get_succ() for a 2-opt exchange
    successors = get_succ(G, pairings, current)
    best_solution = None
    best_cost = float("inf")

    # finds best by comparing them
    for soln in successors:
        soln_cost = total_cost(soln, G)
        if best_solution is None:
            best_solution = soln
            best_cost = soln_cost
        if soln_cost < best_cost:
            best_solution = soln
            best_cost = soln_cost

    return best_cost, best_solution


def hillclimb(G, pairings, max_time, trfilename, best_so_far, seed):
    #Uses hill climbing to find local optimal solution
    start_time = time.time()
    random.seed(seed)
    current = list(np.random.permutation(G.nodes()))
    cost = total_cost(current, G)
    trfile = open(trfilename, 'a')

    if (time.time() - start_time) >= max_time:
        return cost, current

    temp_cost, next_soln = next_solution(G, current, pairings)

    if temp_cost >= cost:
        return cost, current # the best solution has been reached

    cost = temp_cost
    current = next_soln

    if cost < best_so_far:
        trfile.write(str(time.time() - start_time) + ", " + str(cost)+"\n")
        best_so_far = cost

    return cost, current


def hctour(G, trfilename, max_time, seed):
    # Calls hillclimb() a max_iter number of times or until max_time. Returns a locally optimal solution
    # Results improve the more times it is called
    start_time = time.time()
    iterations = 0

    best_soln = None
    best_cost = np.inf

    # gets all possible pairs of nodes
    pairings = pairings_twoxc(G)
    best_so_far = best_cost

    # calls hillclimbing multiple times to find a solution
    if (time.time()-start_time) >= max_time:
        return best_soln, best_cost

    new_cost, new_soln = hillclimb(G, pairings, max_time, trfilename, best_so_far, seed)

    # if new solution is better than old solution, set new solution as best solution
    if best_soln is None:
        best_soln = new_soln
        best_cost = new_cost
    if new_cost < best_cost:
        best_cost = new_cost
        best_soln = new_soln
    #makes it a cycle
    best_soln.append(best_soln[0])

    return best_soln, best_cost

def run_hill_climb(filename, max_time, seed):

    loc = os.path.basename(filename).split('.')[0]
    solfileout =  loc + "_LS1_" + str(max_time) + "_" + str(seed) + ".sol"
    trfileout = loc + "_LS1_" + str(max_time) + "_" + str(seed) + ".trace"

    solfile = open(solfileout, 'w')
    G = create_graph(filename)
    best_sol, best_cost = hctour(G, trfileout, max_time, seed)
    solfile.write(str(best_cost) + "\n")
    tour_str = ""
    for node in best_sol:
        tour_str += str(int(node)) + ", "
    tour_str = tour_str[:(len(tour_str) - 1)]  # delete the last comma
    solfile.write(tour_str + "\n")
