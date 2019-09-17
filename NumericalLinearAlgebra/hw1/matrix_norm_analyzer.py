import sys
import numpy as np
from utils import plot_norm

class MatrixNormAnalyzer(object):
	"""MatrixNormAnalyzer Class"""
	def __init__(self):
		super(MatrixNormAnalyzer, self).__init__()

	def compute_norm(self, L = None, p = 2):

		"""
		Compute p-norm of the input matrix L.

		input: 
			L: the input matrix
			p: p-norm
		output:
			computed value of norm
		"""

		assert (p == -1 or p == 1 or p == 2), "Entered p value is not supported."

		if p == -1:
			row_sum = np.sum(np.absolute(L), axis=1)
			return np.max(row_sum)
		elif p == 1:
			col_sum = np.sum(np.absolute(L), axis=0)
			return np.max(col_sum)
		else:
			s = np.linalg.svd(L, compute_uv=False)
			return s[0]
		

	def analyze(self, num_exp = 100):

		"""
		Analyze the 1-norm, 2-norm, infinity-norm, and conditional number of 2-norm of lower triangular matrix with various dims
		
		input: 
			num_exp: number of repeated experiment for each parameter setting
		output: 
			none
		"""

		avg_rs = []
		avg_conds = []
		avg_norms = {1:[], 2:[], -1:[]}
		sizes = [m for m in range(100, 2100, 100)]

		for m in sizes:
	
			avg_r = []
			avg_cond = []
			avg_norm = {1:[], 2:[], -1:[]}

			for _ in range(int(num_exp)):

				# initialize a m \times m lower triangular matrix ~ Normal(0, \sqrt(m))
				L = np.tril(np.random.normal(0, np.sqrt(m), (m, m)))

				# compute 1-norm
				one_norm = self.compute_norm(L = L, p = 1)

				# compute 2-norm 
				two_norm = self.compute_norm(L = L, p = 2)
				two_norm_inv = self.compute_norm(L = np.linalg.inv(L), p = 2)

				# compute infinite-norm
				inf_norm = self.compute_norm(L = L, p = -1)

				avg_norm[1].append(one_norm)
				avg_norm[2].append(two_norm)
				avg_norm[-1].append(inf_norm)

				# compute ratio of 2_norm and infinite norm
				avg_r.append(two_norm / inf_norm)

				# compute 2_norm conditional number
				avg_cond.append(two_norm * two_norm_inv)

			avg_rs.append(np.mean(np.array(avg_r)))
			avg_conds.append(np.mean(np.array(avg_cond)))
			avg_norms[1].append(np.mean(np.array(avg_norm[1])))
			avg_norms[2].append(np.mean(np.array(avg_norm[2])))
			avg_norms[-1].append(np.mean(np.array(avg_norm[-1])))
			print("Finish " + str(m/100.0) + "%")

		plot_norm(x = sizes, y = avg_norms[1], x_label = "size of matrix", y_label = "||L||_1", name = "l1_norm")
		plot_norm(x = sizes, y = avg_norms[2], x_label = "size of matrix", y_label = "||L||_2", name = "l2_norm")
		plot_norm(x = sizes, y = avg_norms[-1], x_label = "size of matrix", y_label = "||L||_inf", name = "l_inf_norm")
		plot_norm(x = sizes, y = avg_rs, x_label = "size of matrix", y_label = "||L||_2/||L||_inf", name = "norm_ratio")
		plot_norm(x = sizes, y = avg_conds, x_label = "size of matrix", y_label = "conditional number", name = "conditional_number")


def main(argv):
	
	num_exp = 100
	if argv[1]:
		num_exp = argv[1]

	analyzer = MatrixNormAnalyzer()
	analyzer.analyze(num_exp)

if __name__ == '__main__':
	main(sys.argv)