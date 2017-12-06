/* propagate.cl */
__kernel void propagate(
	__global float4* oldPosition,
	__global float4* newPosition,
	__global float4* oldVelocity,
	__global float4* newVelocity,
	const float tau,
	const float A,
	const float a,
	const float B,
	const float b,
	const float C,
	const float c,
	const float timeStep
)
{
  // get the global id for this work item
	int gid = get_global_id(0);

	// pick the particle for which we will calculate the new position
	float4 pos = oldPosition[gid];
	float4 vel = oldVelocity[gid];

	// initilize local flow velocity to 0
	float4 flow_vel = (float4)(0.0, 0.0, 0.0, 0.0);

	// calculate local flow velocity
	flow_vel.x = A * cos(a * (pos.x + (3.14159 / (a * 2)))) * sin(b * (pos.y + (3.14159 / (a * 2)))) * sin(c * (pos.z + (3.14159 / (a * 2))));
	flow_vel.y = B * sin(a * (pos.x + (3.14159 / (a * 2)))) * cos(b * (pos.y + (3.14159 / (a * 2)))) * sin(c * (pos.z + (3.14159 / (a * 2))));
	flow_vel.z = C * sin(a * (pos.x + (3.14159 / (a * 2)))) * sin(b * (pos.y + (3.14159 / (a * 2)))) * cos(c * (pos.z + (3.14159 / (a * 2))));

	// velocity loop
	vel.x += (timeStep / tau) * (flow_vel.x - vel.x);
	vel.y += (timeStep / tau) * (flow_vel.y - vel.y) + (-0.001 * timeStep);
	vel.z += (timeStep / tau) * (flow_vel.z - vel.z);

	// displacement loop
	pos.x += (timeStep * (vel.x + oldVelocity[gid].x) / 2);
	pos.y += (timeStep * (vel.y + oldVelocity[gid].y) / 2);
	pos.z += (timeStep * (vel.z + oldVelocity[gid].z) / 2);

	// update values
	newPosition[gid] = pos;
	newVelocity[gid] = vel;
}
