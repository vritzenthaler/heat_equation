import cython_heat

import numpy as np
import matplotlib.pyplot as plt

x = 0.5
y = 0
z = 1

xmin = 0
xmax = 1
Nx = 100
tmax = 0.01
Nt = 1000

plt.xlim(xmin,xmax)
plt.ylim(0,1.05)
x = np.linspace(xmin, xmax, Nx+1)
plt.plot(x, cython_heat.PySolveHeat(xmin, xmax, Nx, tmax, Nt), 'ro--')
plt.show()
