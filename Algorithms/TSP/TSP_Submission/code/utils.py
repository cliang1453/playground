# branch and bound util files
import sys
import numpy as np
import math
PI = 3.141592
RRR = 6378.388


def data_loader(path):
	with open(path, 'rb') as f:

		# Read the number of vertices and edges
		while True:
			line = f.readline().split()
			if 'DIMENSION'.encode() in line[0]:
				V = int(line[1].decode())
			elif 'EDGE_WEIGHT_TYPE'.encode() in line[0]:
				weight_type = line[1].decode()
			elif 'NODE_COORD_SECTION'.encode() in line[0]:
				break

		if V is None or weight_type is None:
			raise ValueError("Dataset incomplete")

		pos_dict = {}
		for i in range(V):
		    v, x, y = f.readline().split()
		    pos_dict[int(v.decode())] = np.array([float(x.decode()), float(y.decode())])


		matrix = []
		for i in range(V):
			matrix.append([0]*V)

		for i in range(V):
			for j in range(V):
				
				
				if weight_type == 'EUC_2D':
					if i==j:
						matrix[i][j] = math.inf
					else:
						matrix[i][j] = np.round(np.linalg.norm(pos_dict[i+1] - pos_dict[j+1]))
				else:
					if i==j:
						matrix[i][j] = math.inf
					else:
						deg = int(pos_dict[i+1][0])
						min_ = pos_dict[i+1][0]- deg
						rad_xi = PI * (deg + 5.0 * min_/ 3.0) / 180.0 

						deg = int(pos_dict[i+1][1])
						min_ = pos_dict[i+1][1]- deg
						rad_yi = PI * (deg + 5.0 * min_/ 3.0) / 180.0 

						deg = int(pos_dict[j+1][0])
						min_ = pos_dict[j+1][0]- deg 
						rad_xj = PI * (deg + 5.0 * min_/ 3.0) / 180.0

						deg = int(pos_dict[j+1][1]) 
						min_ = pos_dict[j+1][1] - deg
						rad_yj = PI * (deg + 5.0 * min_/ 3.0) / 180.0;

						q1 = np.cos( rad_yi - rad_yj ) 
						q2 = np.cos( rad_xi - rad_xj ) 
						q3 = np.cos( rad_xi + rad_xj )
						matrix[i][j] = int( RRR * np.arccos( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0)


		return np.array(matrix), V

def data_writer(result_list=None, result_dist=None, time=None, instance=None, cutoff=None):
	
	if result_list is not None: 
		with open(instance + '_BnB_' + str(cutoff) + ".sol", 'w') as f:
			f.write("%d\n"%result_dist)
			for v in result_list:
				f.write("%d, "%(v))
	else:
		with open(instance + '_BnB_' + str(cutoff) + ".trace", 'a') as f:
			f.write("%f "%time)
			f.write("%d\n"%result_dist)
            
