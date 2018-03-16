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


def agg_property_variation(path, prefix):
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
                sizes.append(agg.get_size())
            mean = np.mean(sizes)
            means.append(mean)
            std = np.std(sizes)
            stds.append(std)
            property_file.write("{0},{1},{2}\n".format(time, mean, std))
            num_calculated += 1
            if num_calculated % 10 == 0:
                property_file.flush()
    property_file.close()
    plt.plot(times, means)
    plt.plot(times, stds)


agg_property_variation("../cmake-build-release-gcc/TGV_BOX_COHESION/", "100000_TGV_BOX_")
plt.show()
