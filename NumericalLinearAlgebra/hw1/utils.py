import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def plot(self, x = None, y = None, x_label = "", y_label = "", name = ""):
	"""
	Plot the input statistics.

	input: 
		x: list[int]
	    y: list[int]
	    x_label: str
	    y_label: str
	    name: str
	output:
		none
	"""
	plt.plot(x, y)
	plt.xlabel(x_label)
	plt.ylabel(y_label)
	plt.title(y_label + " vs. " + x_label)
	plt.savefig(name + ".png")
	plt.close()