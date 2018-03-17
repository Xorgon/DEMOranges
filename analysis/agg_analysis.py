import numpy as np
from scipy.spatial import Delaunay
from analysis.util.vector_utils import *
from analysis.util.objects import *
from analysis.util.file_io import *
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


def save_agg_property_variation(path, prefix, min_agg_size=1):
    times = []
    means = []
    stds = []
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
            sizes = []
            for agg in aggs:
                if agg.get_size() >= min_agg_size:
                    sizes.append(agg.get_size())
            if len(sizes) > 0:
                mean = np.mean(sizes)
                means.append(mean)
                std = np.std(sizes)
                stds.append(std)
                property_file.write("{0},{1},{2}\n".format(time, mean, std))
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
        data.append([float(split[0]), float(split[1]), float(split[2])])
    data = np.array(data)
    data.sort(0)

    fig, ax1 = plt.subplots()
    fig.patch.set_facecolor('white')
    ax1.plot(data[:, 0], data[:, 1], 'r', label="Label")
    ax1.set_ylabel("Mean", color='r')
    ax1.set_xlabel("$t$")
    handles, labels = ax1.get_legend_handles_labels()
    ax1.legend(handles, labels, loc=2)
    ax2 = ax1.twinx()
    ax2.plot(data[:, 0], data[:, 2], 'b')
    ax2.set_ylabel("Standard Deviation", color='b')
    plt.plot()
    plt.plot()


save_agg_property_variation("../runs/TGV Cohesion Box/TGV_BOX/", "10000_TGV_BOX_", 1)
graph_agg_property_variation("../runs/TGV Cohesion Box/TGV_BOX/", "10000_TGV_BOX_")
plt.show()
