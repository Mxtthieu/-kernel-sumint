__kernel void simple_function(__global int* value, __global int* result) {
	int id = get_global_id(0);
    atomic_add(&result[0], value[id]);
}