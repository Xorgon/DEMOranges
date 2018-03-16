import numpy as np


class Agglomerate:
    id = None
    particles = None

    def __init__(self, id):
        self.id = id
        self.particles = []

    def get_size(self):
        return len(self.particles)


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
