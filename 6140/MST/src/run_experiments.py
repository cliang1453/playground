#!/usr/bin/python
##  CSE6140 HW2
##  This assignment requires installation of networkx package if you want to make use of available graph data structures or you can write your own!!
##  Please feel free to modify this code or write your own
import networkx as nx
import time
import sys
import queue

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

    def dfs(self, curr, u, v, visited):
        
        if len(curr)>0 and curr[-1][1] == v:
            return curr

        for neighbor in self.MST[u]:
            if neighbor[0] in visited:
                continue
            
            curr.append((u, neighbor[0], neighbor[1]))
            visited.add(neighbor[0])
            result = self.dfs(curr, neighbor[0], v, visited)
            if result is not None:
                return result
            visited.remove(neighbor[0])
            curr.pop()

    def recomputeMST(self, u, v, weight, G):
        
        curr = []
        result = self.dfs(curr, u, v, set([u]))
        result.append((u, v, weight))
        result = sorted(result, key=lambda item: item[2])
        u_to_remove, v_to_remove, weight_to_remove = result[-1]
        self.MSTweight = self.MSTweight - weight_to_remove + weight

        if u_to_remove != u or v_to_remove != v or weight_to_remove != weight:
            self.MST[u_to_remove].remove((v_to_remove, weight_to_remove))
            self.MST[v_to_remove].remove((u_to_remove, weight_to_remove))
            self.MST[u].add((v, weight))
            self.MST[v].add((u, weight))

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
        output.write(str(MSTweight) + " " + str(total_time) + "\n")


        #Changes file
        self.MST = {}
        for n in range(self.num_nodes):
            self.MST[n] = set([])

        for edge in self.T:
            self.MST[edge[0]].add((edge[1], edge[2]))
            self.MST[edge[1]].add((edge[0], edge[2]))

        #self.weight_check = [self.MSTweight]


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

                # write new weight and time to output file
                # self.weight_check.append(new_weight)
                output.write(str(new_weight) + " " + str(total_recompute) + "\n")

        # with open('../results/rmat0608.out', 'r') as f:
        #     for i in range(self.num_edges + 1):
        #         w, t = [l for l in f.readline().split()]
        #         if self.weight_check[i] != int(w):
        #             print(i)
        # print('correct')





if __name__ == '__main__':
    # run the experiments
    runexp = RunExperiments()
    runexp.main()
