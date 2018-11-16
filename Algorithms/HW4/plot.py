import matplotlib.pyplot as plt
import sys
import time
import dc
import dp

def main():

	ave_dc_dict = []
	ave_dp_dict = []
	nums = [10, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000]
	for num in nums:
		
		file_name = 'data/' + str(num) + '.txt'
		ave_dc = 0
		ave_dp = 0
		
		with open(file_name, 'r') as f:
			# Read the number of vertices and edges
			n, k = [int(l) for l in f.readline().split(',')]

			for cnt in range(k):
				arr = [float(l) for l in f.readline().split(',')]
				
				start = time.time()
				max_sum, indices = dc.maxSubArraySum(arr, 0, n-1) 
				ave_dc += time.time() - start

				start = time.time()
				max_sum, indices = dp.maxSubArraySum(arr) 
				ave_dp = time.time() - start

			ave_dc = ave_dc/k
			ave_dp = ave_dp/k
		
		ave_dc_dict.append(ave_dc)
		ave_dp_dict.append(ave_dp)


	plt.plot(nums, ave_dc_dict)
	plt.plot(nums, ave_dp_dict)
	plt.legend(['divide_and_conquer', 'dynamic programming'], loc='upper left')
	plt.ylabel('average running time')
	plt.xlabel('length of array')
	plt.show()

if __name__ == '__main__':
	main()