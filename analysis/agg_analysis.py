import numpy as np
from scipy.spatial import Delaunay
from analysis.util.vector_utils import *
from analysis.util.objects import *
from analysis.util.file_io import *
from analysis.util.sim_utils import *
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import os
import re


def detect_agglomerates(particles):
    aggs = []
    points = []
    for p in particles:
        points.append(p.pos)

    tri = Delaunay(points)

    for p in particles:
        if p.agg_id is not None:
            continue  # Particle is already in an Agglomerate

        agg = Agglomerate(len(aggs))
        aggs.append(agg)
        agg.particles.append(p)
        p.agg_id = agg.id

        growing = True
        while growing:
            growing = False
            for agg_p in agg.particles:
                ns = tri.vertex_neighbor_vertices
                n_p_idxs = ns[1][ns[0][agg_p.idx]:ns[0][agg_p.idx + 1]]
                for n_p_idx in n_p_idxs:
                    n_p = particles[n_p_idx]
                    if n_p.agg_id is None:
                        if mag_squared(agg_p.pos - n_p.pos) <= ((agg_p.diameter + n_p.diameter) / 2) ** 2:
                            n_p.agg_id = agg.id
                            agg.particles.append(n_p)
                            growing = True
    return aggs


def calc_size_stats(aggs, min_agg_size):
    sizes = []
    for agg in aggs:
        if agg.get_size() >= min_agg_size:
            sizes.append(agg.get_size())
    if len(sizes) > 0:
        mean = np.mean(sizes)
        std = np.std(sizes)
        return mean, std
    else:
        print("No agglomerates found.")
        return None


def calc_void_frac_stats(aggs, min_agg_size):
    void_fracs = []
    for agg in aggs:
        if agg.get_size() >= min_agg_size:
            void_fracs.append(agg.get_void_fraction())
    if len(void_fracs) > 0:
        mean = np.mean(void_fracs)
        std = np.std(void_fracs)
        return mean, std
    else:
        print("No agglomerates found.")
        return None


def check_bounds(particles, domain_length):
    outside = 0
    for p in particles:
        if fabs(p.pos[0]) > domain_length / 2 or fabs(p.pos[1]) > domain_length or fabs(p.pos[2] > domain_length):
            outside += 1
    return outside


def save_agg_property_variation(path, prefix, min_agg_size=1, override=False):
    times = []
    size_means = []
    size_stds = []
    void_frac_means = []
    void_frac_stds = []

    if not os.path.exists(path):
        print(path + " does not exist.")
        return

    data_dir = os.listdir(path)

    if os.path.exists(path + prefix + "agglomerate_properties.txt") and not override:
        print(path + " has already been analysed.")
        return

    print("Analysing " + path)

    property_file = open(path + prefix + "agglomerate_properties.txt", "w")
    num_calculated = 0
    for filename in data_dir:
        name_match = re.match(prefix + "(\d+).txt", filename)
        if name_match:
            time = int(name_match.group(1)) / 1e6
            print("Time = " + str(time))
            times.append(time)
            particles = load_particles(path, filename)
            out_of_bounds = check_bounds(particles, 2 * pi)
            if out_of_bounds > 0:
                print(str(out_of_bounds) + " particles are out of bounds.")
            aggs = detect_agglomerates(particles)
            size_ret = calc_size_stats(aggs, min_agg_size)
            void_frac_ret = calc_void_frac_stats(aggs, min_agg_size)
            if size_ret is not None and void_frac_ret is not None:
                size_mean, size_std = size_ret
                size_means.append(size_mean)
                size_stds.append(size_std)

                void_frac_mean, void_frac_std = void_frac_ret
                void_frac_means.append(void_frac_mean)
                void_frac_stds.append(void_frac_std)

                property_file.write(
                    "{0},{1},{2},{3},{4}\n".format(time, size_mean, size_std, void_frac_mean, void_frac_std))

                num_calculated += 1
            if num_calculated % 10 == 0:
                property_file.flush()
    property_file.close()


def graph_agg_property_variation(path, prefix, title=True):
    if not os.path.exists(path):
        print(path + " does not exist.")
        return

    if not os.path.exists(path + prefix + "agglomerate_properties.txt"):
        print(path + prefix + " has not been analysed.")
        return

    property_file = open(path + prefix + "agglomerate_properties.txt", "r")
    lines = property_file.readlines()
    data = []
    for line in lines:
        split = line.split(",")
        data.append([float(split[0]), float(split[1]), float(split[2]), float(split[3]), float(split[4])])
    data = np.array(data)
    data.sort(0)

    stokes = get_stokes_number(path, prefix)
    sticky = get_stickyness_number(path, prefix)

    fig = plt.figure(figsize=(8, 10))
    fig.patch.set_facecolor('white')
    ax1 = fig.add_subplot(211)
    if title:
        ax1.set_title("Stokes = {0:2f}, Stickyness = {1:2f}".format(stokes, sticky))
    ax1.plot(data[:, 0], data[:, 1], 'k', label="Size Mean")
    ax1.set_ylabel("Mean")
    ax1.set_xlabel("$t$")
    ax2 = ax1.twinx()
    ax2.plot(data[:, 0], data[:, 2], 'k--', label="Size Standard Deviation")
    ax2.set_ylabel("Standard Deviation")
    handles1, labels1 = ax1.get_legend_handles_labels()
    handles2, labels2 = ax2.get_legend_handles_labels()
    ax1.legend(handles1 + handles2, labels1 + labels2, loc=4)

    ax3 = fig.add_subplot(212)
    ax3.plot(data[:, 0], data[:, 3], 'k', label="Void Fraction Mean")
    ax3.set_ylabel("Mean")
    ax3.set_xlabel("$t$")
    ax4 = ax3.twinx()
    ax4.plot(data[:, 0], data[:, 4], 'k--', label="Void Fraction Standard Deviation")
    ax4.set_ylabel("Standard Deviation")
    handles3, labels3 = ax3.get_legend_handles_labels()
    handles4, labels4 = ax4.get_legend_handles_labels()
    ax3.legend(handles3 + handles4, labels3 + labels4, loc=4)
    plt.plot()


def get_representative_data(path, prefix, min=500, max=550):
    if not os.path.exists(path):
        print(path + " does not exist.")
        return

    if not os.path.exists(path + prefix + "agglomerate_properties.txt"):
        print(path + prefix + " has not been analysed.")
        return

    property_file = open(path + prefix + "agglomerate_properties.txt", "r")
    lines = property_file.readlines()
    data = []
    for line in lines:
        split = line.split(",")
        data.append([float(split[0]), float(split[1]), float(split[2]), float(split[3]), float(split[4])])
    data = np.array(data)
    data.sort(0)

    size_sum = 0
    void_frac_sum = 0
    count = 0

    for item in data:
        if min <= item[0] <= max:
            size_sum += item[1]
            void_frac_sum += item[3]
            count += 1

    size_mean = size_sum / count
    void_frac_mean = void_frac_sum / count

    stokes = get_stokes_number(path, prefix)
    sticky = get_stickyness_number(path, prefix)

    return stokes, sticky, size_mean, void_frac_mean


def plot_3d_sy_stk_variation():
    points = []
    for i in [-1, 0, 1, 2]:
        for j in [-1, 0, 1, 2]:
            data = get_representative_data("../runs/Multi/TGV_PERIODIC_{0}_{1}/".format(i, j), "10000_TGV_PERIODIC_")
            if data is not None:
                points.append(data)

    points = np.array(points)
    fig = plt.figure()
    fig.patch.set_facecolor('white')
    ax = fig.gca(projection='3d')
    ax.plot_trisurf(points[:, 0], points[:, 1], points[:, 2], cmap=cm.coolwarm)
    ax.set_xlabel("Stokes")
    ax.set_ylabel("Stickiness")
    ax.set_zlabel("Mean Size")

    fig = plt.figure()
    fig.patch.set_facecolor('white')
    ax = fig.gca(projection='3d')
    ax.plot_trisurf(points[:, 0], points[:, 1], points[:, 3], cmap=cm.coolwarm)
    ax.set_xlabel("Stokes")
    ax.set_ylabel("Stickiness")
    ax.set_zlabel("Mean Void Fraction")


def plot_num_cols(path, prefix):
    if not os.path.exists(path):
        print(path + " does not exist.")
        return

    if not os.path.exists(path + prefix + "cols.txt"):
        print(path + prefix + "cols.txt does not exist.")
        return

    cols_file = open(path + prefix + "cols.txt", "r")
    lines = cols_file.readlines()
    num_cols = []
    times = []
    for line in lines:
        split = line.split(",")
        num_cols.append(int(split[1]))
        times.append(float(split[0]))

    stokes = get_stokes_number(path, prefix)
    sticky = get_stickyness_number(path, prefix)

    fig = plt.figure()
    fig.patch.set_facecolor('white')
    ax = fig.gca()
    ax.plot(times, num_cols)
    ax.set_title("Stokes = {0:2f}, Stickyness = {1:2f}".format(stokes, sticky))


def get_mean_sy_stk_num_cols(path, prefix):
    if not os.path.exists(path):
        print(path + " does not exist.")
        return

    if not os.path.exists(path + prefix + "cols.txt"):
        print(path + prefix + "cols.txt does not exist.")
        return

    cols_file = open(path + prefix + "cols.txt", "r")
    lines = cols_file.readlines()
    num_cols = []
    times = []
    for line in lines:
        split = line.split(",")
        num_cols.append(int(split[1]))
        times.append(float(split[0]))

    stokes = get_stokes_number(path, prefix)
    sticky = get_stickyness_number(path, prefix)

    return sticky, stokes, np.mean(num_cols)


def plot_mean_sy_num_cols(stk=1):
    points = []
    for i in [-1, 0, 1, 2]:
        for j in [stk]:
            data = get_mean_sy_stk_num_cols("../runs/Multi/TGV_PERIODIC_{0}_{1}/".format(i, j), "10000_TGV_PERIODIC_")
            if data is not None:
                points.append(data)

    points = np.array(points)
    points = np.sort(points, 0)

    fig = plt.figure()
    fig.patch.set_facecolor('white')
    ax = fig.gca()
    ax.plot(points[:, 0], points[:, 2], "x-")
    ax.set_title("Stokes = {0:2f}".format(points[0, 1]))


for i in [-1, 0, 1, 2]:
    for j in [-1, 0, 1, 2]:
        save_agg_property_variation("../runs/Multi/TGV_PERIODIC_{0}_{1}/".format(i, j), "10000_TGV_PERIODIC_", 1)
        plot_num_cols("../runs/Multi/TGV_PERIODIC_{0}_{1}/".format(i, j), "10000_TGV_PERIODIC_")
        # graph_agg_property_variation("../runs/Multi/TGV_PERIODIC_{0}_{1}/".format(i, j), "10000_TGV_PERIODIC_")

graph_agg_property_variation("../runs/Multi/TGV_PERIODIC_{0}_{1}/".format(1, 2), "10000_TGV_PERIODIC_", False)
graph_agg_property_variation("../runs/Multi/TGV_PERIODIC_{0}_{1}/".format(-1, 0), "10000_TGV_PERIODIC_", False)
plot_3d_sy_stk_variation()
# plot_mean_sy_num_cols(1)

plt.show()
