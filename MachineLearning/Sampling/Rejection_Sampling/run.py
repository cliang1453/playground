from scipy.stats import cauchy
from scipy.stats import gamma
import matplotlib.pyplot as plt
import numpy as np

z = 2.7693
M = np.pi * (z + np.power(z, 3)) * np.exp(-z)
print(M)
fig, ax = plt.subplots(1, 1)
x = np.linspace(0,
                5, 100)
c = cauchy()
ax.plot(x, M* c.pdf(x), 'k-', color='orange', lw=2, label='cauchy pdf')
y = np.linspace(0,
    5, 100)
g = gamma(2, loc = 0., scale = 1)
ax.plot(y, g.pdf(y), 'k-', color='green', lw=2, label='gamma pdf')
legend = ax.legend(loc='upper right', shadow=True, fontsize='x-large')
plt.show()