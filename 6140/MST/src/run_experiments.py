#!/usr/bin/python
##  CSE6140 HW2
##  This assignment requires installation of networkx package if you want to make use of available graph data structures or you can write your own!!
##  Please feel free to modify this code or write your own
import networkx as nx
import time
import sys

class RunExperiments:

    def find_root(self, V, n):
        
        if V[n] == n: 
            return n 
        return self.find_root(V, V[n]) 

    def union(self, V, rank, u, v):
        if rank[u] < rank[v]: 
            V[u] = v 
        elif rank[u] > rank[v]: 
            V[v] = u  
        else : 
            V[v] = u 
            rank[u] += 1

    def dfs(self, u, v, result, curr, start):
        if curr


        self.MST[u]

    def recomputeMST(self, u, v, weight, G):
        
        result = []
        curr = []
        self.dfs(u, v, result, curr, 0)
        sorted(Cycle, key=lambda item: item[2])
        self.MSTweight = self.MSTweight - Cycle[0][2] + weight
        
        if Cycle[0][1] == u and Cycle[0][2] == v:
            self.MST[u].append(v)
            self.MST[v].append(u)

        return self.MSTweight

    def computeMST(self, G):

        G = sorted(G, key=lambda item: item[2]) 
        V = [n for n in range(self.num_nodes)] # list that contains singleton nodes 
        rank = [0]*self.num_nodes
        self.T = []
        self.MSTweight = 0

        for i in range(self.num_edges):
            
            u, v, weight = G[i] 
            u_root = self.find_root(V, u) 
            v_root = self.find_root(V ,v) 
  
            if u_root != v_root:  
                self.T.append((u, v, weight))    
                self.MSTweight += weight
                self.union(V, rank, u_root, v_root) 

            if len(self.T) == self.num_nodes-1:
                break        

        return self.MSTweight

    def read_graph(self, filename):
        #make sure to add to filename the directory where it is located and the extension .txt
        
        #if you want to use networkx
        # G = nx.MultiGraph()
        #Write code to add nodes and edges
        #Check out add_node, add_edge in networkx
        G = []
        with open(filename, 'rb') as f:
            # Read the number of vertices and edges
            self.num_nodes, self.num_edges = [int(l) for l in f.readline().split()]
            for edge in range(self.num_edges):
                u, v, weight = [int(l) for l in f.readline().split()]
                G.append((u, v, weight))
        return G


    def main(self):

        num_args = len(sys.argv)

        if num_args < 4:
            print("error: not enough input arguments")
            exit(1)

        graph_file = sys.argv[1]
        change_file = sys.argv[2]
        output_file = sys.argv[3]

        #Construct graph
        G = self.read_graph(graph_file)
        self.MSTweight = 0

        start_MST = time.time() #time in seconds
        MSTweight = self.computeMST(G) #call MST function to return total weight of MST
        total_time = (time.time() - start_MST) * 1000 #to convert to milliseconds

        #Write initial MST weight and time to file
        output = open(output_file, 'w')
        output.write(str(MSTweight) + " " + str(total_time))


        #Changes file
        self.MST = {}
        for edge in self.T:
            self.MST[edge[0]].append((egde[1], edge[2]))
            self.MST[edge[1]].append((egde[0], edge[2]))

        with open(change_file, 'r') as changes:
            num_changes = changes.readline()

            for line in changes:
                #parse edge and weight
                edge_data = list(map(lambda x: int(x), line.split()))
                assert(len(edge_data) == 3)

                u,v,weight = edge_data[0], edge_data[1], edge_data[2]

                #call recomputeMST function
                start_recompute = time.time()
                new_weight = self.recomputeMST(u, v, weight, G)
                total_recompute = (time.time() - start_recompute) * 1000 # to convert to milliseconds

                #write new weight and time to output file
                output.write(str(new_weight) + " " + str(total_recompute) + "\n")



if __name__ == '__main__':
    # run the experiments
    runexp = RunExperiments()
    runexp.main()
