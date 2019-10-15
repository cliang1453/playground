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

def plot_runtime(x, y):
	plt.plot(x, y)
	plot_labels(x_label="size of toeplitz matrix", y_label="runtime", 
				title="Runtime vs. Size of toeplitz matrix for efficient \
				Householder triangularization implementation", \
				name="Q4", legend=False)