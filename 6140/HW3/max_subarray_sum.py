# A Divide and Conquer based program 
# for maximum subarray sum problem 

# Find the maximum possible sum in 
# arr[] auch that arr[m] is part of it 
def maxCrossingSum(arr, l, m, h) : 
	
	# Include elements on left of mid. 
	sm = 0; left_sum = -10000
	left_idx = m
	right_idx = m

	for i in range(m, l-1, -1) : 
		sm = sm + arr[i] 
		
		if (sm > left_sum) : 
			left_sum = sm 
			left_idx = i
	
	
	# Include elements on right of mid 
	sm = 0; right_sum = -1000
	for i in range(m + 1, h + 1) : 
		sm = sm + arr[i] 
		
		if (sm > right_sum) : 
			right_sum = sm 
			right_idx = i 
	

	# Return sum of elements on left and right of mid 
	return left_sum + right_sum, (left_idx, right_idx)


# Returns sum of maxium sum subarray in aa[l..h] 
def maxSubArraySum(arr, l, h) : 
	
	# Base Case: Only one element 
	if (l == h) : 
		return arr[l], (l, l)

	# Find middle point 
	m = (l + h) // 2

	# Return maximum of following three possible cases 
	# a) Maximum subarray sum in left half 
	# b) Maximum subarray sum in right half 
	# c) Maximum subarray sum such that the 
	#	 subarray crosses the midpoint 

	# indices inclusive!

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


# Driver Code 
arr = [2, 18, -22, 20, 8, -6, 10, -24, 13, 3] 
n = len(arr) 

max_sum, indices = maxSubArraySum(arr, 0, n-1) 
print("Maximum contiguous sum is ", max_sum)
print(indices) 

# This code is contributed by Nikita Tiwari. 
