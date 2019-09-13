import matplotlib.pyplot as plt
x1 = [1 * 10**6, 2 * 10**6, 3*10**6, 4 * 10**6, 5 * 10**6]
y1 = [239856, 500959, 758980, 1038700, 1327310]
plt.plot(x1, y1, label = "thread")
plt.xlabel('x-axix')
plt.ylabel('y-axix')
plt.legend()
plt.show()