import sys
import time

def maxCrossingSum(arr, l, m, h): 
	 
	sm = 0; left_sum = -10000
	left_idx = m
	right_idx = m

	for i in range(m, l-1, -1) : 
		sm = sm + arr[i] 
		
		if (sm > left_sum) : 
			left_sum = sm 
			left_idx = i
	
	
	sm = 0; right_sum = -1000
	for i in range(m + 1, h + 1) : 
		sm = sm + arr[i] 
		
		if (sm > right_sum) : 
			right_sum = sm 
			right_idx = i 
	
	return left_sum + right_sum, (left_idx, right_idx)

 
def maxSubArraySum(arr, l, h) : 
	
	# Base Case
	if (l == h) : 
		return arr[l], (l, l)

	# Find middle point 
	m = (l + h) // 2

	left_sum, left_indices = maxSubArraySum(arr, l, m)
	right_sum, right_indices = maxSubArraySum(arr, m+1, h)
	cross_sum, cross_indices = maxCrossingSum(arr, l, m, h)

	max_sum = max(left_sum, right_sum, cross_sum)
	if max_sum == left_sum:
		return left_sum, left_indices
	elif max_sum == right_sum:
		return right_sum, right_indices
	else:
		return cross_sum, cross_indices


def main(argv):

	file_name = argv[1]
	result = []

	with open(file_name, 'r') as f:
		# Read the number of vertices and edges
		n, k = [int(l) for l in f.readline().split(',')]

		for cnt in range(k):
			arr = [float(l) for l in f.readline().split(',')]
			start = time.time()
			max_sum, indices = maxSubArraySum(arr, 0, n-1) 
			elapse = time.time() - start
			result.append((max_sum, indices[0]+1, indices[1]+1, elapse))

	with open('output/cliang73_output_dc_' + str(n) + '.txt', 'w') as f:
		for cnt in range(k):
			r = result[cnt]
			f.write(str(r[0]) + ',' + str(r[1]) + ',' + str(r[2]) + ',' + str(r[3]) + '\n')

				

if __name__ == '__main__':
	main(sys.argv)