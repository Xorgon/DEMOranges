import re
from analysis.util.file_io import *
from math import *


def get_mass(density, diameter):
    return density * pi * diameter ** 3 / 6


def get_stokes_number(path, prefix, flow_speed=0.7839 * 5, length=pi / 3):
    property_file = open(path + prefix + "setup.txt")
    contents = property_file.read()
    p_diameter = get_property("Particle diameter", contents)
    p_density = get_property("Particle density", contents)
    fluid_visc = get_property("Fluid viscosity", contents)

    return p_density * p_diameter ** 2 * flow_speed / (18 * fluid_visc * length)


def get_stickyness_number(path, prefix, flow_speed=0.7839 * 5):
    property_file = open(path + prefix + "setup.txt")
    contents = property_file.read()
    restitution = get_property("Restitution coefficient", contents)
    cohesion_stiffness = get_property("Cohesion stiffness", contents)
    p_diameter = get_property("Particle diameter", contents)
    effect_diameter = get_property("Effect diameter", contents)
    if effect_diameter is None:
        effect_diameter = 1.5 * p_diameter
    p_density = get_property("Particle density", contents)

    e_len = effect_diameter - p_diameter
    m = get_mass(p_density, p_diameter)

    return e_len * cohesion_stiffness ** 0.5 / (
            restitution * (e_len ** 2 * cohesion_stiffness + flow_speed ** 2 * m) ** 0.5)
