import matplotlib.pyplot as plt

fig = plt.figure(figsize=(10, 8))
fig.patch.set_facecolor('white')
ax = fig.add_subplot(111)

ns = [10000, 100000, 200000, 300000, 400000, 500000, 700000, 1000000, 2500000, 5000000, 7500000, 10000000]
times = [0.166667, 1.266667, 2.5166667, 3.63333, 4.8833333, 6.183333333, 8.6166667, 12.63333333, 32.5833333, 66.35, 102.533333, 145.2166666]

ax.plot(ns, times, 'k', label="Benchmarked Times")
ax.plot([10000, 10000000], [0.166667, 10000000*(12.633333333-0.166667)/(1000000-10000)], 'k--', label="Linear Extrapolation")
ax.plot(ns, times, 'rx')
handles, labels = ax.get_legend_handles_labels()
ax.legend(handles, labels)
ax.set_xlabel("Number of Particles (N)")
ax.set_ylabel("Run time (minutes)")
plt.show()