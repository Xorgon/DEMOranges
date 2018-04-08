from math import *
import numpy as np
import matplotlib.pyplot as plt
import os
import re


def calculate_damping_coefficient(stiffness, restitution, m1, m2):
    ln_rest = log(restitution)
    return -2 * ln_rest * (get_reduced_particle_mass(m1, m2) * stiffness / (pi ** 2 + ln_rest ** 2)) ** 0.5


def get_mass(density, diameter):
    return density * pi * diameter ** 3 / 6


def get_reduced_particle_mass(m1, m2):
    return m1 * m2 / (m1 + m2)


def get_incoming_pos(t, m, k_c, d_e, u_0):
    return u_0 * sqrt(m / k_c) * sinh(t * sqrt(k_c / m)) + d_e


def get_colliding_pos(t, m, damping_coeff, k_e, k_c, u_i, d_b, d_e):
    a = - damping_coeff / (2 * m)
    b = sqrt(4 * m * k_e - damping_coeff ** 2) / (2 * m)
    c = k_c * (d_e - d_b) / (k_e - k_c)
    return exp(a * t) * (sin(b * t) * (u_i - a * c) / b + c * cos(b * t)) + (k_e * d_b - k_c * d_e) / (k_e - k_c)


def get_returning_pos(t, m, k_c, d_b, d_e, u_r):
    return (d_b - d_e) * cosh(sqrt(k_c / m) * t) + u_r * sqrt(m / k_c) * sinh(sqrt(k_c / m) * t) + d_e


def get_incoming_vel(t, m, k_c, u_0):
    return u_0 * cosh(t * sqrt(k_c / m))


def get_colliding_vel(t, m, damping_coeff, k_e, k_c, u_i, d_b, d_e):
    a = - damping_coeff / (2 * m)
    b = sqrt(4 * m * k_e - damping_coeff ** 2) / (2 * m)
    c = k_c * (d_e - d_b) / (k_e - k_c)

    return exp(a * t) * (((a * u_i - a * c) / b - c * b) * sin(b * t) + u_i * cos(b * t))


def get_returning_vel(t, m, k_c, d_b, d_e, u_r):
    return sqrt(k_c / m) * (d_b - d_e) * sinh(sqrt(k_c / m) * t) + u_r * cosh(sqrt(k_c / m) * t)


def plot_pos_and_vel():
    stiffness = 1e5
    cohesion_stiffness = 1e2
    density = 2000
    diameter = 0.1
    effect_diameter = 1.5 * diameter
    mass = get_mass(density, diameter)
    restitution = 0.8
    u_0 = -2

    damping_coeff = calculate_damping_coefficient(stiffness, restitution, mass, 1e308)

    col_duration = pi * sqrt(mass / stiffness)

    # Generate Analytic Solution Data
    times = np.arange(0, col_duration * 4, col_duration / 500)
    positions = []
    velocities = []

    t_i = sqrt(mass / cohesion_stiffness) * asinh(sqrt(cohesion_stiffness / mass) * (diameter - effect_diameter) / u_0)
    u_i = u_0 * cosh(t_i * sqrt(cohesion_stiffness / mass))

    u_r = None
    t_r = None
    for t in times:
        incoming_pos = get_incoming_pos(t, mass, cohesion_stiffness, effect_diameter, u_0)
        if incoming_pos > diameter:
            positions.append(incoming_pos)
            velocities.append(get_incoming_vel(t, mass, cohesion_stiffness, u_0))
        else:
            if positions[-1] > diameter:
                u_i = velocities[-1]
            colliding_pos = get_colliding_pos(t - t_i, mass, damping_coeff, stiffness, cohesion_stiffness, u_i, diameter,
                                              effect_diameter)
            if colliding_pos < diameter and u_r is None:
                positions.append(colliding_pos)
                velocities.append(
                    get_colliding_vel(t - t_i, mass, damping_coeff, stiffness, cohesion_stiffness, u_i, diameter,
                                      effect_diameter))
            else:
                if positions[-1] < diameter and u_r is None:
                    u_r = velocities[-1]
                    t_r = times[np.where(times == t)[0][0] - 1]
                returning_pos = get_returning_pos(t - t_r, mass, cohesion_stiffness, diameter, effect_diameter, u_r)
                positions.append(returning_pos)
                vel = get_returning_vel(t - t_r, mass, cohesion_stiffness, diameter, effect_diameter, u_r)
                velocities.append(vel)

    # Get Simulation Data
    data = []
    increments = []
    data_dir = os.listdir("data")
    for filename in data_dir:
        name_match = re.match("2_cohesion_(\d+)_\d+.txt", filename)
        if name_match:
            i = int(name_match.group(1))
            if increments.count(i) == 0:
                increments.append(i)
    increments.sort()
    for i in increments:
        timestep_data = np.array([]).reshape((0, 3))  # Times, Positions, Velocities
        for filename in data_dir:
            name_match = re.match("2_cohesion_" + str(i) + "_(\d+).txt", filename)
            if name_match:
                time = int(name_match.group(1)) / 1e6
                file = open("data/" + filename, "r")
                line = file.readlines()[1]
                data_match = re.match("(\d+\.\d+),(\d+\.\d+),(\d+\.\d+),(-?\d+\.\d+),(\d+\.\d+),(\d+\.\d+)", line)
                if data_match and time <= col_duration * 3.99:
                    timestep_data = np.append(timestep_data,
                                              [[time, float(data_match.group(1)), float(data_match.group(4))]], axis=0)
        timestep_data = timestep_data[timestep_data[:, 0].argsort()]  # Sort by time
        data.append(timestep_data)

    # Normalize Data
    for incr_data in data:
        for n in range(len(incr_data)):
            incr_data[n, 0] = incr_data[n, 0] / col_duration  # Normalize time with collision duration
            incr_data[n, 1] = incr_data[n, 1] / diameter  # Normalize position with particle diameter
            incr_data[n, 2] = incr_data[n, 2] / u_0  # Normalize velocity with initial velocity

    for n in range(len(positions)):
        times[n] = times[n] / col_duration
        positions[n] = positions[n] / diameter
        velocities[n] = velocities[n] / u_0

    # Plot Data
    fig = plt.figure(figsize=(8, 10))
    fig.patch.set_facecolor('white')
    ax1 = fig.add_subplot(211)
    ax1_lines = []
    analytic_line, = ax1.plot(times, positions, 'k', label="Analytic")
    ax1_lines.append(analytic_line)
    for i in range(len(data)):
        timestep_data = data[i]
        timestep_label = "$t_{collision} / " + str(increments[i]) + "$"
        line, = ax1.plot(timestep_data[:, 0], timestep_data[:, 1], '--', label=timestep_label)
        ax1_lines.append(line)
    ax1.set_xlabel('$t / t_{collision}$')
    ax1.set_ylabel('$x / d_p$')
    ax1.legend(handles=ax1_lines, loc=1)

    ax2 = fig.add_subplot(212)
    ax2_lines = []
    analytic_line, = ax2.plot(times, velocities, 'k', label="Analytic")
    ax2_lines.append(analytic_line)
    for i in range(len(data)):
        timestep_data = data[i]
        timestep_label = "$t_{collision} / " + str(increments[i]) + "$"
        line, = ax2.plot(timestep_data[:, 0], timestep_data[:, 2], '--', label=timestep_label)
        ax2_lines.append(line)
    ax2.set_ylabel('$u / u_0$')
    ax2.set_xlabel('$t / t_{collision}$')
    ax2.legend(handles=ax2_lines, loc=1)

    fig.tight_layout()
    plt.show()


plot_pos_and_vel()
