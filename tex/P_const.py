import matplotlib.pyplot as plt
x = [5,10,15,20,25]
x1 = [332023,
161565.2,
114880.7,
91456.94,
62021.44]

plt.plot(x,x1)

plt.scatter(x,x1)
plt.grid()
plt.xlabel("C")
plt.ylabel("$T_{avg}$ in $\mu$ sec")
plt.legend()
plt.show()
