kernel void ArrayMult( global const float *dA, global const float *dB, global float *dC )
{
	int gid = get_global_id( 0 );

	dC[gid] = dA[gid] * dB[gid];
}

kernel void ArrayMultSum( global const float *dA, global const float *dB, global float *dC, global const float *dD )
{
	int gid = get_global_id( 0 );

	dC[gid] = dA[gid] * dB[gid] + dD[gid];
}
