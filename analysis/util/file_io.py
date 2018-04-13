from analysis.util.objects import *
import re


def load_particles(path, filename):
    file = open(path + filename, 'r')
    lines = file.readlines()
    particles = []
    for line in lines:
        split = line.split(",")
        diameter = 0.05
        if 3 < len(split) < 6:
            diameter = float(split[3])
        pos = [float(split[0]), float(split[1]), float(split[2])]
        particle = Particle(len(particles), pos, diameter)
        particles.append(particle)

    return particles


def get_property(name, file_contents):
    pattern = name + ": (\d+.\d+)\n"
    match = re.search(pattern, file_contents)
    if match is not None:
        return float(match.group(1))
    else:
        print("Warning: " + name + " is None.")
        return None
