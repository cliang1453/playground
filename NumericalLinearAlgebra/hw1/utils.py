import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def plot_labels(x_label = "", y_label = "", title = "", name = ""):

	if title == "":
		title = y_label + " vs " + x_label

	plt.xlabel(x_label)
	plt.ylabel(y_label)
	plt.title(title)
	plt.legend()
	plt.savefig(name + ".png")
	plt.close()


def plot_norm(x = None, y = None, x_label = "", y_label = "", title = "", name = ""):

	plt.plot(x, y)
	plot_labels(x_label, y_label, title, name)
	


def plot_solutions(params = None, num_discretization = None, solution = None, exact_solution = None, mode = "Pivoting"):

	for lam in params["b"]:

		for i in len(num_discretization):
			n = num_discretization[i]
			x = np.array(params["xb1"], params["xb2"], (params["xb2"] - params["xb1"]) / (n + 1))
			y = solution[lam][i]
			plt.plot(x, y, label= "# interior mesh points =" + str(n))

		name = mode + " GE solution using centered difference scheme vs Exact solution for 1D nonhomogeneous ODE at lambda = " + str(lam)
		plot_labels("x", "u(x)", name, name.split(' ').join('_'))		


def plot_sol_comparison(params = None, num_discretization = None, solution_naive = None, solution_pivoting = None):

	for i in num_discretization:
		
		n = num_discretization[i]
		x = np.array(params["xb1"], params["xb2"], (params["xb2"] - params["xb1"]) / (n + 1))

		for lam in params["b"]:
			
			y_naive = solution_naive[lam][i]
			y_pivoting = solution_pivoting[lam][i]

			plt.plot(x, y_naive, label= "naive GE solution, lambda =" + str(lam))
			plt.plot(x, y_pivoting, label= "pivoting GE solution, lambda =" + str(lam))

		name = "Comparison of naive GE solution vs pivoting GE solution using centered difference scheme for 1D nonhomogeneous ODE with # interior mesh points = " + str(n)
		plot_labels("x", "u(x)", name, name.split(' ').join('_'))


def plot_runtime_comparison(params = None, num_discretization = None, runtime_naive = None, runtime_pivoting = None):

	for lam in params["b"]:
		
		y_naive = runtime_naive[lam]
		y_pivoting = runtime_pivoting[lam]

		plt.plot(num_discretization, y_naive, label= "naive GE solution, lambda =" + str(lam))
		plt.plot(x, y_pivoting, label= "pivoting GE solution, lambda =" + str(lam))
		
	name = "Runtime comparison of naive GE vs pivoting GE solution using centered difference scheme for 1D nonhomogeneous ODE at lambda = " + str(lam)
	plot_labels("# interior mesh points", "runtime (ms)", name, name.split(' ').join('_'))