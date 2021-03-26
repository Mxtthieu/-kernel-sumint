__kernel void simple_function(__global int* value, __global int* result, int K) {
	int id = get_global_id(0);
	int res = 0;
	for(int i = 0; i < K; i++) {
	    int tmp = id * K + i;
	    res += value[tmp];
    }
    atomic_add(&result[0], res);
}