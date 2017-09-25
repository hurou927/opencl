
typedef struct{
	unsigned int x,y,z;
} IDX;


__kernel void matmul(	__global int* matrix,
						__global int* vector,
						__global int* result){

	IDX threadIdx = {get_local_id(0),get_local_id(1),get_local_id(2)};
	//IDX blockIdx  = {get_group_id(0),get_group_id(1),get_group_id(2)};
	//IDX blockDim  = {get_local_size(0),get_local_size(1),get_local_size(2)};
	//IDX gridDim   = {get_num_groups(0),get_num_groups(1),get_num_groups(2)};
	IDX globalIdx = {get_global_id(0),get_global_id(1),get_global_id(2)};

	__local int A[256];
	__local int B[256];


	int l_id = threadIdx.x + threadIdx.y * 16;
	int i,j;
	int sum = 0;
	for(j=0;j<64;j++){
		// global -> shared
		A[l_id] = matrix[(threadIdx.x + j) + globalIdx.y * 1024];
		B[l_id] = vector[globalIdx.x + (threadIdx.y + j) * 1024];

		barrier(CLK_LOCAL_MEM_FENCE);

		//Compute
		for(i=0;i<16;i++){
			sum += A[threadIdx.y * 16 + i] * B[threadIdx.x + i * 16];
		}
	}

	result[globalIdx.x+globalIdx.y*1024]=sum;
	
	//printf("Hello, World GroupID:%2d GlobalID:%2d LocalID:%2d\n",get_group_id(0),get_global_id(0),get_local_id(0));
}
