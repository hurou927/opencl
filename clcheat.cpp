#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

#include "my_gettime.hpp"
#include "my_cl_util.hpp"
#include "my_clobj.hpp"




int main(int argc, char **argv){
    //PrintDeviceQuery();
    timeStamp ts(10,"ms");

    clLikeCUDA clo;//qeury:clo(-1)
    clo.printTargetInfo(); //clo.printTargetInfoDetail();
    cl_kernel kernel1 = clo.clCreateKernelFromFile((char *)"matmul.cl",(char *)"matmul",(char *)"");
    //cl_kernel kernel2 = clo.clCreateKernelFromFile((char *)"matmul.cl",(char *)"matmul",(char *)"");

    cl_mem *d_a;
    clo.clMalloc(&d_a,sizeof(int)*1024*1024,CL_MEM_READ_ONLY);
    cl_mem *d_b;
    clo.clMalloc(&d_b,sizeof(int)*1024*1024,CL_MEM_READ_ONLY);
    cl_mem *d_c;
    clo.clMalloc(&d_c,sizeof(int)*1024*1024,CL_MEM_READ_WRITE);


    #if __cplusplus >= 201103L  //-std=c++11
        clo.clSetKernelArgs(kernel1, d_a, d_b, d_c);
    #else
        clSetKernelArg(kernel1,0,sizeof(cl_mem),d_a);
        clSetKernelArg(kernel1,1,sizeof(cl_mem),d_b);
        clSetKernelArg(kernel1,2,sizeof(cl_mem),d_c);
    #endif

    int *h_a=(int *)malloc(sizeof(int)*1024*1024);
    int *h_b=(int *)malloc(sizeof(int)*1024*1024);
    int *h_c=(int *)malloc(sizeof(int)*1024*1024);
    for(int i=0;i<1024*1024;i++){
        h_a[i]=1;
        h_b[i]=1;
        h_c[i]=2;
    }


    cldim3 blocks(64,64);
    cldim3 threads(16,16);



    ts.stamp();
    clo.clMemcpy(d_a,h_a,sizeof(int)*1024*1024,clMemcpyHostToDevice);
    clo.clMemcpy(d_b,h_b,sizeof(int)*1024*1024,clMemcpyHostToDevice);
    clo.clMemcpy(d_c,h_c,sizeof(int)*1024*1024,clMemcpyHostToDevice);
    ts.stamp();

    cl_int err;
    err = clo.runkernel(kernel1,blocks,threads);

    //err = clEnqueueNDRangeKernel(clo.queue,kernel1,2,NULL,threadnum_per_grid,threadnum_per_block,0,NULL,NULL);
    //clo.Finish();
    //clo.waitLastEvent();

    ts.stamp();

    clo.clMemcpy(h_c,d_c,sizeof(int)*1024*1024,clMemcpyDeviceToHost);

    ts.stamp();

    if(err!=CL_SUCCESS){
        printf("clEnqueueNDRangeKerne error %s\n",getErrorString(err));
        exit(EXIT_FAILURE);
    }


    for(int i=0;i<1024*1024;i++){
        if(h_c[i]!=1024){
            printf("Error: %d %d\n",i,h_c[i]);
            break;
        }
    }

    ts.print();

    clReleaseMemObject(*d_a);
    clReleaseMemObject(*d_b);
    clReleaseMemObject(*d_c);

    return 0;
}
