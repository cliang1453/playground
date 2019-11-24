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

def plot_runtime_method(m_range, mu_range, methods, times):
	
	for method in methods:
		for mu in mu_range:
			plt.plot(m_range, times[method][mu], label=method + ", " + str(mu))
		plot_labels(x_label="size of matrix A", y_label="runtime", 
					title="Runtime vs. Size of matrix A for " + method, \
					name="runtime_" + method, legend=True)

def plot_runtime_mu(m_range, mu_range, methods, times):

	for mu in mu_range:
		for method in methods:
			plt.plot(m_range, times[method][mu], label=method + ", " + str(mu))
		plot_labels(x_label="size of matrix A", y_label="runtime", 
					title="Runtime vs. Size of matrix A for " + str(mu), \
					name="runtime_" + str(mu), legend=True)

def plot_err_method(m_range, mu_range, methods, err):
	
	for method in methods:
		for mu in mu_range:
			plt.plot(m_range, err[method][mu], label=method + ", " + str(mu))
		plot_labels(x_label="size of matrix A", y_label="l2 error", 
					title="L2 error vs. Size of matrix A for " + method, \
					name="error_" + method, legend=True)

def plot_err_mu(m_range, mu_range, methods, err):

	for mu in mu_range:
		for method in methods:
			plt.plot(m_range, err[method][mu], label=method + ", " + str(mu))
		plot_labels(x_label="size of matrix A", y_label="l2 error", 
					title="L2 error vs. Size of matrix A for " + str(mu), \
					name="error_" + str(mu), legend=True)