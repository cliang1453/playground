
with open('../results/total_time_result.txt', 'r') as f:
	edges = []
	static_times = []
	dy_times = []
	for line in range(39):
		if line%3 == 0:
			edge = f.readline()
			edges.append(int(edge))
		elif line%3 == 1:
			static_time = f.readline()
			static_times.append(float(static_time))
		else:
			dy_time = f.readline()
			dy_times.append(float(dy_time))
print(edges)
print(static_times)
print(dy_times)

import matplotlib.pyplot as plt 
# plt.figure(1)
# plt.plot(edges, static_times)
# plt.title('number of initial edges v.s. computation time')
# plt.xlabel('number of initial edges')
# plt.ylabel('computation time')
# plt.show()

plt.figure(1)
plt.plot([16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536], dy_times)
plt.title('number of vertices/edges in static MST v.s. recomputation time for an additional 1000 edges')
plt.xlabel('number of vertices/edges in static MST')
plt.ylabel('recomputation time for an additional 1000 edges')
plt.show()