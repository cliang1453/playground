from numpy import np
import sys
import glob
import math


name = ['Berlin', 'Boston', 'Cincinnati', 'ulysses16']
opt_sol = {'Berlin': 7542;
		   'ulysses16': 6859;
		   'Boston': 893536;
		   'Cincinnati': 277952}
q = [5, 10, 15]
city_dict = {} #(#success out of 10, []*10)


def main(argv):

	dataset_path = argv[1:]
	for f in glob.glob( dataset_path + '*.trace'):
		city, method, cutoff, roun = f.split("_")
		cutoff = int(cutoff)
		if city not in city_dict:
			city_dict[city] = (0, [math.inf]*10)
		


if __name__ == '__main__':
	main(sys.args)