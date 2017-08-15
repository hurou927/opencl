#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_gettime.hpp"
#include "my_clobj.hpp"


int main(int argc, char **argv){

    timeStamp ts(10);

    // select platform and device
    //clLikeCUDA clo;
	clLikeCUDA clo(-1); //stdio selection mode

    //print target device
    clo.printTargetInfo();
    //clo.printTargetInfoDetail();

    //build source
    char fileName[] = "matmul.cl";
    char funcName[] = "matmul";
    char options[]  = "";
    cl_kernel kernel1 = clo.clCreateKernelFromFile(fileName, funcName, options);

    //malloc device memory
    cl_mem *d_a;
    clo.clMalloc(&d_a,sizeof(int)*1024*1024,CL_MEM_READ_ONLY);
    cl_mem *d_b;
    clo.clMalloc(&d_b,sizeof(int)*1024*1024,CL_MEM_READ_ONLY);
    cl_mem *d_c;
    clo.clMalloc(&d_c,sizeof(int)*1024*1024,CL_MEM_READ_WRITE);

    //set the arguments
    #if __cplusplus >= 201103L  //-std=c++11
        clo.clSetKernelArgs(kernel1, d_a, d_b, d_c);
    #else
        clSetKernelArg(kernel1,0,sizeof(cl_mem),d_a);
        clSetKernelArg(kernel1,1,sizeof(cl_mem),d_b);
        clSetKernelArg(kernel1,2,sizeof(cl_mem),d_c);
    #endif

    //malloc host memory
    int *h_a=(int *)malloc(sizeof(int)*1024*1024);
    int *h_b=(int *)malloc(sizeof(int)*1024*1024);
    int *h_c=(int *)malloc(sizeof(int)*1024*1024);
    for(int i=0;i<1024*1024;i++){
        h_a[i]=1;
        h_b[i]=1;
        h_c[i]=2;
    }

    //blocks (x,y,z) = (64,64)
    //threads (x,y,z) = (16,16)
    cldim3 blocks(64,64);  //equal to (64,64,1)
    cldim3 threads(16,16); //equal to (16,16,1)

    //memory copy from host to device
    ts.stamp();
    clo.clMemcpy(d_a,h_a,sizeof(int)*1024*1024,clMemcpyHostToDevice);
    clo.clMemcpy(d_b,h_b,sizeof(int)*1024*1024,clMemcpyHostToDevice);
    clo.clMemcpy(d_c,h_c,sizeof(int)*1024*1024,clMemcpyHostToDevice);
    ts.stamp();

    //run kernel *blocking(sync) execution
    cl_int err;
    err = clo.runkernel(kernel1,blocks,threads);

    ts.stamp();

    //memory copy from host to device
    clo.clMemcpy(h_c,d_c,sizeof(int)*1024*1024,clMemcpyDeviceToHost);

    ts.stamp();

    if(err!=CL_SUCCESS){
        printf("clEnqueueNDRangeKerne error %s\n",getErrorString(err));
        exit(EXIT_FAILURE);
    }

/*
    for(int i=0;i<1024*1024;i++){
        if(h_c[i]!=1024){
            printf("Error: %d %d\n",i,h_c[i]);
            break;
        }
    }
*/
//    ts.print();
    printf("--------------------------------------\n");
    printf("[ Matrix Multiplication(1024 x 1024) ]\n");
    printf("host->device, %6.3f,ms\n",ts.interval(0,1)*1000);
    printf("kernel      , %6.3f,ms\n",ts.interval(1,2)*1000);
    printf("device->host, %6.3f,ms\n",ts.interval(2,3)*1000);

    clReleaseMemObject(*d_a);
    clReleaseMemObject(*d_b);
    clReleaseMemObject(*d_c);

    return 0;
}
