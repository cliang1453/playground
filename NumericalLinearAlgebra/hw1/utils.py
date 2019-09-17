import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from textwrap import wrap

def plot_labels(x_label = "", y_label = "", title = "", name = "", legend = True):

	if title == "":
		title = y_label + " vs " + x_label

	plt.xlabel(x_label)
	plt.ylabel(y_label)
	plt.title("\n".join(wrap(title, 60)), fontsize=10)
	if legend:
		plt.legend()
	plt.savefig(name + ".png")
	plt.close()


def plot_norm(x = None, y = None, x_label = "", y_label = "", title = "", name = ""):

	plt.plot(x, y)
	plot_labels(x_label, y_label, title, name, False)
	

def plot_error_comparison(params = None, num_discretization = None, solution_naive = None, solution_pivoting = None, exact_solution = None):

	for lam in params["b"]:
		
		error_naive, error_pivoting = [], []
		for i in range(len(num_discretization)):
		
			n = num_discretization[i]
			h = (params["xb2"] - params["xb1"]) / (n + 1)
			x = np.arange(params["xb1"] + h, params["xb2"], h)

			y_naive = solution_naive[lam][i]
			y_pivoting = solution_pivoting[lam][i]
			if lam == 0:
				c1, c2, c3, c4 = exact_solution[lam]
				y_exact = c1 * x ** 4 + c2 * x ** 2 + c3 * x + c4
			else:
				c1, c2, c3, c4, c5, c6 = exact_solution[lam]
				y_exact = c1 * x ** 2 + c2 * np.exp(c3 * x) + c4 * np.exp(c5 * x) + c6

			error_naive.append(np.linalg.norm(y_naive - y_exact))
			error_pivoting.append(np.linalg.norm(y_pivoting - y_exact))

		plt.plot(num_discretization, error_naive, label= "l2 error of naive GE")
		plt.plot(num_discretization, error_pivoting, label= "l2 error of pivoting GE")
		name = "Comparison of l2 error of naive GE solution vs pivoting GE solution using centered difference scheme for 1D nonhomogeneous ODE at lambda=" + str(lam)
		plot_labels("# interior mesh points", "l2 error", name, "_".join(name.split(' ')))


def plot_solutions(params = None, num_discretization = None, solution = None, exact_solution = None, mode = "Pivoting"):

	h_exact = (params["xb2"] - params["xb1"]) / (4000 + 1)
	x_exact = np.arange(params["xb1"] + h_exact, params["xb2"], h_exact)

	for lam in params["b"]:

		if lam == 0:
			c1, c2, c3, c4 = exact_solution[lam]
			y_exact = c1 * x_exact ** 4 + c2 * x_exact ** 2 + c3 * x_exact + c4
		else:
			c1, c2, c3, c4, c5, c6 = exact_solution[lam]
			y_exact = c1 * x_exact ** 2 + c2 * np.exp(c3 * x_exact) + c4 * np.exp(c5 * x_exact) + c6

		plt.plot(x_exact, y_exact, label= "exact_solution")

		for i in range(len(num_discretization)):
			n = num_discretization[i]
			h = (params["xb2"] - params["xb1"]) / (n + 1)
			x = np.arange(params["xb1"] + h, params["xb2"], h)
			y = solution[lam][i]
			plt.plot(x, y, label= "# interior mesh points=" + str(n))
			

		name = mode + " GE solution using centered difference scheme vs Exact solution for 1D nonhomogeneous ODE at lambda=" + str(lam)
		plot_labels("x", "u(x)", name, "_".join(name.split(' ')))		


def plot_sol_comparison(params = None, num_discretization = None, solution_naive = None, solution_pivoting = None):

	for i in range(len(num_discretization)):
		
		n = num_discretization[i]
		h = (params["xb2"] - params["xb1"]) / (n + 1)
		x = np.arange(params["xb1"] + h, params["xb2"], h)

		for lam in params["b"]:
			
			y_naive = solution_naive[lam][i]
			y_pivoting = solution_pivoting[lam][i]

			plt.plot(x, y_naive, label= "naive GE solution, lambda =" + str(lam))
			plt.plot(x, y_pivoting, label= "pivoting GE solution, lambda =" + str(lam))

		name = "Comparison of naive GE solution vs pivoting GE solution using centered difference scheme for 1D nonhomogeneous ODE with # interior mesh points=" + str(n)
		plot_labels("x", "u(x)", name, "_".join(name.split(' ')))


def plot_runtime_comparison(params = None, num_discretization = None, runtime_naive = None, runtime_pivoting = None):

	for lam in params["b"]:
		
		y_naive = runtime_naive[lam]
		y_pivoting = runtime_pivoting[lam]

		plt.plot(num_discretization, y_naive, label= "naive GE solution, lambda =" + str(lam))
		plt.plot(num_discretization, y_pivoting, label= "pivoting GE solution, lambda =" + str(lam))
		
		name = "Runtime comparison of naive GE vs pivoting GE solution using centered difference scheme for 1D nonhomogeneous ODE at lambda=" + str(lam)
		plot_labels("# interior mesh points", "runtime (ms)", name, "_".join(name.split(' ')))