# MST approximation

import time
import sys
from math import cos, sqrt, acos, floor
import os

def distance(i,j):
    '''Compute distance between two points represented as radians'''
    q1 = cos(i[0] - j[0])
    q2 = cos(i[1] - j[1])
    q3 = cos(i[0] + j[0])
    dist = int(6378.388 * acos(0.5*((1.0+q1)*q2 - (1.0-q1)*q3)) + 1.0)
    return dist


def read_tsp(filename):
    '''Parse tsp files. Takes filename as input and creates an edgelist of a complete graph with weighted edges representing distance between points in a plane.
    Also returns a duplicate edgelist, and the number of points/vertices in graph. Edgelists are stored as dictionaries whose keys are the (x,y) coordinates and 
    and values are distances between these points'''

    global n

    #Berlin graph has more preliminary lines than other tsp files so create special case for it
    if os.path.basename(filename) == 'Berlin.tsp':
        city = {}
        cood = open(filename,'r')
        city_name = cood.readline()
        data_type = cood.readline()
        comment = cood.readline()
        n = int(cood.readline().split(':')[1])
        weight_type = cood.readline()
        node = cood.readline()
        #start collecting and processing node information from here on
        for i in range(n):
            coods = cood.readline().split()
            x, y = float(coods[1]), float(coods[2])
            city[i+1] = (x,y)

        edgelist = {}  #dictionary for undirected complete graph. Contains duplicate edges in reverse from d
        ## Calculate the edge weights between all pairs and populate the edgelists.
        for i in range(1,n+1):
            for j in range(i+1,n+1):
                #compute the Euclidean distance as metric
                metric = int(round(sqrt((city[i][0] - city[j][0])**2 + (city[i][1] - city[j][1])**2)))
                edgelist[(i,j)] = metric
                edgelist[(j,i)] = metric

    #Separate this because data is presented in GEO format and has to be processed separately from other files
    elif os.path.basename(filename) == 'ulysses16.tsp':
        pi = 3.141592
        city = {}
        cood = open(filename,'r')
        city_name = cood.readline()
        data_type = cood.readline()
        comment = cood.readline()
        n = int(cood.readline().split()[1])
        weight_type = cood.readline()
        display_type = cood.readline()
        node = cood.readline()
        #start collecting and processing node information from here onwards
        for i in range(n):
            coods = cood.readline().split()
            x, y = float(coods[1]), float(coods[2])
            # convert display format to radians for computing geographical distance between points
            x_deg = floor(x)
            min_x = x - x_deg
            x = pi*(x_deg + 5*min_x / 3) / 180.
            y_deg = floor(y)
            min_y = y - y_deg
            y = pi*(y_deg + 5*min_y / 3) / 180.
            city[i+1] = (x,y)
        edgelist = {}
        for i in range(1,n+1):
            for j in range(i+1,n+1):
                #metric is the Euclidean distance between two points in graph
                metric = distance(city[i],city[j])
                edgelist[(i,j)] = metric
                edgelist[(j,i)] = metric

    #All other tsp files are processed here.
    else:
        city = {}
        cood = open(filename,'r')
        city_name = cood.readline()
        comment = cood.readline()
        n = int(cood.readline().split(':')[1])
        weight_type = cood.readline()
        node = cood.readline()
        for i in range(n):
            coods = cood.readline().split()
            x, y = float(coods[1]), float(coods[2])
            city[i+1] = (x,y)
        edgelist = {}
        for i in range(1,n+1):
            for j in range(i+1,n+1):
                metric = int(round(sqrt((city[i][0] - city[j][0])**2 + (city[i][1] - city[j][1])**2),0))
                edgelist[(i,j)] = metric
                edgelist[(j,i)] = metric

    #return edgelists and number of cities for a tsp file.
    return (edgelist,n)


def primMST(G, vertex_size, root):
    '''Compute the minimum spanning tree using Prim's algorithm. Take as input a graph edgelist represented as a dictionary, the number of nodes in the graph,
    and the root of the minimum spanning tree. Due to density of graph, priority queue is implemented as an array'''
    keys = vertex_size*[float('inf')]   #Initiate priority queue and set key value of all elements to infinity
    keys[root] = 0                      #Set key value of root to 0 so that it is picked first
    MST = []                            #list to store minimum spanning tree of graph
    explored = set([])
    unexplored = set(list(range(vertex_size)))
    while unexplored: ##keep running if there are more nodes to be added to the spanning tree yet
        curr = keys.index(min(keys))    ##extract-min operation
        explored.add(curr)              ##add node extracted to the minimum spanning tree
        unexplored.discard(curr)        ##remove recently extracted node from the unexplored/undiscovered nodes set
        MST.append(curr +1)             ##Grow the spanning tree by adding the recently extracted node
        for city in unexplored:         ##Update-key method for unextracted nodes
            new_key = G[curr+1, city+1]
            if keys[city] > new_key:
                keys[city] = new_key
        keys[curr] = float('inf')       ##Set key of extracted node to infinity such that it cannot be extracted again by the extract-min operation
    return(MST)


def MSTAppPrim(town, root):
    '''2-approximation algorithm for traveling saleman problem. Takes as input an edgelist and the root of the tree, and outputs a tour, total cost of the tour and the number of vertices in the tour.'''
    edgelist, vertex_size = read_tsp(town)          #parse and process nodes and distance information from tsp file
    pre_tour = primMST(edgelist, vertex_size, root) #compute the minimum spanning tree of the graph rooted at the chosen root node
    l = (len(pre_tour))
    tour = []
    #create a pretour order from the minimum spannig tree by listing the nodes in order in which they are discovered by Prim's algorithm
    for i in range(l):
        tour.append(pre_tour[i])
    tour.append(pre_tour[0])        #append the root node to the pretour to create a tour
    quality =  0            #set counter for length of tour
    for i in range(len(tour)-1):    #compute tour length by summing the weight of edges in the tour cycle
        quality += edgelist[(tour[i],tour[i+1])]
    #return the tour, the cost of the tour and the number of vertices in the tour.
    return(tour, quality, vertex_size)


def run_mst(filename, cutoff_time):

    
    point = os.path.basename(filename).split('.')[0]
    #Create a list to store best ten results obtained from changing starting cities
    best = float('inf')
    d, v_size = read_tsp(filename)
    
    ##Iterate over every city, using each as a root in the minimum spanning tree
    global_start_time = time.time()
    
    for q in range(v_size):

        tour, quality, vertex_size = MSTAppPrim(filename,q)
        
        # if quality is better than the largest quality seen so far, replace the largest by the current solution
        if (quality < best):
            
            best_quality, best_tour = quality, tour

            global_time = time.time() - global_start_time
            r_file = open(str(point) + '_Approx_' + str(cutoff_time) + '.trace', 'a')
            r_file.write(str(global_time) + ' , ' + str(quality) + '\n')
            r_file.close()

            best = quality

        if q > vertex_size:
            break
        
        if time.time() - global_start_time > cutoff_time:
            break

    results_file = open(str(point) + '_Approx_' + str(cutoff_time) + '.sol','w')
    results_file.write(str(best_quality) + '\n' )
    results_file.write(str(best_tour).strip('[]') + '\n')
    results_file.close()