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
	plot_labels(x_label="size of toeplitz matrix", y_label="runtime", 
				title="Runtime vs. Size of toeplitz matrix for efficient \
				Householder triangularization implementation", \
				name="Q4", legend=False)

def plot_runtime_comparison(n_range, times):

	for k, v in times.items():
		if k == "classical":
			plt.plot(n_range, v, label= "Classical Gram Schmidt")
		elif k == "modified":
			plt.plot(n_range, v, label= "Modified Gram Schmidt")
		else:
			plt.plot(n_range, v, label= "Householder Transform")
	
	name = "Runtime v.s matrix size of classical Gram Schmidt, modified Gram Schmidt and Householder Transform algorithms"
	plot_labels(x_label = "matrix size", y_label = "runtime", title = name, name = "Q5_runtime")

def plot_error_comparison(n_range, R_error):

	for k, v in R_error.items():
		if k == "classical":
			plt.plot(n_range, v, label= "Classical Gram Schmidt")
		elif k == "modified":
			plt.plot(n_range, v, label= "Modified Gram Schmidt")
		else:
			plt.plot(n_range, v, label= "Householder Transform")
	
	name = "R error v.s matrix size of classical Gram Schmidt, modified Gram Schmidt and Householder Transform algorithms"
	plot_labels(x_label = "matrix size", y_label = "l2 error", title = name, name = "Q5_error")