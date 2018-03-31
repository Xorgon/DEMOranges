import numpy as np
from scipy.spatial import Delaunay
from analysis.util.vector_utils import *
from analysis.util.objects import *
from analysis.util.file_io import *
from analysis.util.sim_utils import *
from matplotlib import pyplot as plt
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
        return None


def save_agg_property_variation(path, prefix, min_agg_size=1):
    times = []
    size_means = []
    size_stds = []
    void_frac_means = []
    void_frac_stds = []
    data_dir = os.listdir(path)
    property_file = open(path + prefix + "agglomerate_properties.txt", "w")
    num_calculated = 0
    for filename in data_dir:
        name_match = re.match(prefix + "(\d+).txt", filename)
        if name_match:
            time = int(name_match.group(1)) / 1e6
            print("Time = " + str(time))
            times.append(time)
            aggs = detect_agglomerates(load_particles(path, filename))
            size_ret = calc_size_stats(aggs, min_agg_size)
            void_frac_ret = calc_void_frac_stats(aggs, max(min_agg_size, 2))
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


def graph_agg_property_variation(path, prefix):
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
    ax1.set_title("Stokes = {0:2f}, Stickyness = {1:2f}".format(stokes, sticky))
    ax1.plot(data[:, 0], data[:, 1], 'r', label="Size")
    ax1.set_ylabel("Mean", color='r')
    ax1.set_xlabel("$t$")
    handles, labels = ax1.get_legend_handles_labels()
    ax1.legend(handles, labels, loc=2)
    ax2 = ax1.twinx()
    ax2.plot(data[:, 0], data[:, 2], 'b')
    ax2.set_ylabel("Standard Deviation", color='b')

    ax3 = fig.add_subplot(212)
    ax3.plot(data[:, 0], data[:, 3], 'r', label="Void Fraction")
    ax3.set_ylabel("Mean", color='r')
    ax3.set_xlabel("$t$")
    handles, labels = ax3.get_legend_handles_labels()
    ax3.legend(handles, labels, loc=2)
    ax4 = ax3.twinx()
    ax4.plot(data[:, 0], data[:, 4], 'b')
    ax4.set_ylabel("Standard Deviation", color='b')

    plt.plot()


save_agg_property_variation("../runs/TGV_PERIODIC_LONG/", "10000_TGV_PERIODIC_", 1)
graph_agg_property_variation("../runs/TGV_PERIODIC_LONG/", "10000_TGV_PERIODIC_")
# save_agg_property_variation("../runs/TGV_PERIODIC_LONG_2/", "10000_TGV_PERIODIC_", 1)
graph_agg_property_variation("../runs/TGV_PERIODIC_LONG_2/", "10000_TGV_PERIODIC_")

# save_agg_property_variation("../cmake-build-release-gcc/TGV_PERIODIC_3/", "10000_TGV_PERIODIC_", 1)
# graph_agg_property_variation("../cmake-build-release-gcc/TGV_PERIODIC_3/", "10000_TGV_PERIODIC_")
# print(calc_size_stats(detect_agglomerates(load_particles("../cmake-build-release-gcc/TGV_PERIODIC_3/", "10000_TGV_PERIODIC_12491837.txt")), 2))
plt.show()
