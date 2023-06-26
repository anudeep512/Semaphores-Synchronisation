import matplotlib.pyplot as plt
x = [10,15,20,25,30,35,40,45,50]
y1 = [83025.46,
91487.62,
76772.58,
86298.68,
95859.32,
93680.38,
86596.42,
81078.98,
80684.14]
plt.plot(x,y1)
plt.scatter(x,y1)
plt.grid()
plt.xlabel("P")
plt.ylabel("$T_{avg}$ in $\mu$ sec")
plt.legend()
plt.show()
