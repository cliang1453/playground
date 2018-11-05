import sys
import time

def maxSubArraySum(arr):
	
	B = [0]*(len(arr)+1)
	max_idx = 0
	for i in range(1, len(arr)+1):
		B[i] = max(B[j-1] + arr[i-1], 0)
		if B[j] > B[max_idx]:
			max_idx = j

	zero_idx = max_idx
	while zero_idx >= 0 and B[zero_idx] != 0:
		zero_idx -= 1

	return B[max_idx], (zero_idx+1, max_idx)


def main(argv):

	file_name = argv[1]
	result = []

	with open(file_name, 'r') as f:
		# Read the number of vertices and edges
		n, k = [int(l) for l in f.readline().split(',')]

		for cnt in range(k):
			arr = [float(l) for l in f.readline().split(',')]
			start = time.time()
			max_sum, indices = maxSubArraySum(arr) 
			elapse = time.time() - start
			result.append((max_sum, indices[0], indices[1], elapse))

	with open('output/cliang73_output_dp_' + str(n) + '.txt', 'w') as f:
		for cnt in range(k):
			r = result[cnt]
			f.write(str(r[0]) + ',' + str(r[1]) + ',' + str(r[2]) + ',' + str(r[3]) + '\n')

if __name__ == '__main__':
	main(sys.argv)