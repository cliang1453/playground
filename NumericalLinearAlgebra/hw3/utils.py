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

def plot_runtime(m_range, times):
	plt.plot(m_range, times)
	plot_labels(x_label="size of matrix A", y_label="runtime", 
				title="Runtime vs. Size of matrix A for QR decomposition using Givens Rotation algorithm", \
				name="Q4", legend=False)

def plot_runtime_comparison(As, times, params, num_discretization):

	for lam in params["b"]:
		plt.plot(num_discretization, times[lam], label= "runtime, lambda =" + str(lam))
		
	name = "Runtime v.s. num_discretization"
	plot_labels("number of discretization", "runtime (ms)", name, "Q5")