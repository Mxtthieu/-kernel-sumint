__kernel void simple_function(__global int* value, __global int* result) {
	int id = get_global_id(0);
	int wid = get_group_id(0);
    atomic_add(&result[wid], value[id]);
}