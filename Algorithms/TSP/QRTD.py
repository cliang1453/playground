import numpy as np
import sys
import glob
import math
import matplotlib.pyplot as plt


name = ['Berlin', 'Boston', 'Cincinnati']
opt_sol = {'Berlin': 7542,
		   'Boston': 893536,
		   'Cincinnati': 277952}

q = 0.25
city_dict = {} #(#success out of 10, []*10)
cut_off = 3.0
cut = 1000
time_dict = {}
l = list(np.arange(0, cut_off, cut_off/cut))
for i in l:
	time_dict[i] = 0


def main():

	opt_bar = {}
	for city, dist in opt_sol.items():
		opt_bar[city] = opt_sol[city]*(1 + q)
	
	for filename in glob.glob( 'SA/Berlin*.trace'):
		f = open(filename, 'r') 
		print(filename)
		i = 0
		while i < len(l):
			
			while True:
				line = f.readline()
				if line == "":
					break 

				t, d = line.split(' ')
				if float(t) <= l[i]:
					last_d = int(d)
				else:
					while float(t) > l[i]:
						i += 1
					break


			if line == "" or i == len(l):
				break

			
			if last_d < opt_bar['Berlin']:
				for add_k in l[i:]:
					time_dict[add_k] += 1
				break


	# for key in range(1, cut):
	# 	time_dict[cut_off/cut*key] += time_dict[cut_off/cut*(key-1)]

	# norm = time_dict[(cut-1)/cut*cut_off]
	for key, val in time_dict.items():
		time_dict[key] = float(val)/10.0
		#print(time_dict[key])

	plt.plot(*zip(*sorted(time_dict.items())))
	plt.xlabel('Runtime CPU Seconds(s)')
	plt.ylabel('P(solved)')
	plt.title('QRTD for Simulated Annealing at q = 0.25')
	plt.show()

if __name__ == '__main__':
	main()






			

		


if __name__ == '__main__':
	main(sys.args)