/* Kernel to make collision list for particle-particle collisions. */

/*
For all CVs, go through all the particles and all the neighbours.
Only create a collision if the particle being collided with others has a pid less than the other.
This is essentially what a naive approach does but for a limited number of particles.
*/

__kernel void make_pp_collisions(
) {}
