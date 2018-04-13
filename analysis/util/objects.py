import numpy as np
import analysis.util.vector_utils as vect
import math


class Agglomerate:
    id = None
    particles = None

    def __init__(self, id):
        self.id = id
        self.particles = []

    def get_size(self):
        return len(self.particles)

    def get_max_length(self):
        max_dist = 0
        for p1 in self.particles:
            for p2 in self.particles:
                if p1 == p2:
                    continue
                else:
                    dist = vect.mag(vect.subtract(p1.pos, p2.pos)) + p1.diameter / 2 + p2.diameter / 2
                    if dist > max_dist:
                        max_dist = dist
        return max_dist

    def get_void_fraction(self):
        if self.get_size() == 1:
            return 0
        volume_filled = 0
        bounding_sphere_volume = math.pi * self.get_max_length() ** 3 / 6
        for particle in self.particles:
            volume_filled += particle.get_volume()
        return 1 - volume_filled / bounding_sphere_volume


class Particle:
    idx = None
    pos = None
    diameter = None
    agg_id = None

    def __init__(self, idx, pos, diameter):
        self.idx = idx
        self.pos = np.array(pos)
        self.diameter = diameter

    def set_agg_id(self, agg_id):
        self.agg_id = agg_id

    def get_volume(self):
        return math.pi * self.diameter ** 3 / 6
