__kernel void matmul(	__global int* matrix,
						__global int* vector,
						__global int* result){

	__local int A[256];
	__local int B[256];
	int i,j,sum=0;

	int g_x,g_y,l_x,l_y;
	g_x=get_global_id(0);
	g_y=get_global_id(1);
	l_x=get_local_id(0);
	l_y=get_local_id(1);

	int l_id=l_x+l_y*16;

	for(j=0;j<64;j++){
		A[l_id]=matrix[(l_x+j) +g_y*1024];
		B[l_id]=vector[g_x+ (l_y+j)*1024];

		barrier(CLK_LOCAL_MEM_FENCE);

		for(i=0;i<16;i++){
			sum += A[l_y*16+i]*B[l_x+i*16];
		}
	}

	result[g_x+g_y*1024]=sum;
	//printf("Hello, World GroupID:%2d GlobalID:%2d LocalID:%2d\n",get_group_id(0),get_global_id(0),get_local_id(0));
}
