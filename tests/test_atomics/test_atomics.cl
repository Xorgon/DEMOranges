__kernel void test_atomics(__global particle *particles){
    atomicAdd_float3(&particles[0].pos, (float3){1, 2, 3});
}