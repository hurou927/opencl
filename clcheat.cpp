#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/my_gettime.hpp"
#include "header/my_clobj.hpp"

#define OPT1 (10)

int main(int argc, char **argv){

    timeStamp ts(10);
    
    int matrixWidth = 1024; //Square

    // select platform and device
    //clLikeCUDA clo;      // default platform and device
    //clLikeCUDA clo(1);   // device number
    clLikeCUDA clo(-1);  // stdin selection mode

    //print target device
    clo.printTargetInfo();
    //clo.printTargetInfoDetail();

    //build source
    std::string fileName = "matmul.cl";
    std::string funcName = "matmul";
    std::string options = createOption("OPT1",OPT1);

    cl_kernel kernel1 = clo.clCreateKernelFromFile(fileName, funcName, options);

    //malloc device memory
    cl_mem *d_a;
    clo.clMalloc(&d_a,sizeof(int)*matrixWidth*matrixWidth,CL_MEM_READ_ONLY);
    cl_mem *d_b;
    clo.clMalloc(&d_b,sizeof(int)*matrixWidth*matrixWidth,CL_MEM_READ_ONLY);
    cl_mem *d_c;
    clo.clMalloc(&d_c,sizeof(int)*matrixWidth*matrixWidth,CL_MEM_READ_WRITE);

    //set the arguments
    
    #if __cplusplus >= 201103L  //-std=c++11
        clo.clSetKernelArgs(kernel1, d_a, d_b, d_c, &matrixWidth);
    #else
        clSetKernelArg(kernel1,0,sizeof(cl_mem),d_a);
        clSetKernelArg(kernel1,1,sizeof(cl_mem),d_b);
        clSetKernelArg(kernel1,2,sizeof(cl_mem),d_c);
        clSetKernelArg(kernel1,3,sizeof(int),&matrixWidth);
    #endif

    //malloc host memory
    int *h_a=(int *)malloc(sizeof(int)*matrixWidth*matrixWidth);
    int *h_b=(int *)malloc(sizeof(int)*matrixWidth*matrixWidth);
    int *h_c=(int *)malloc(sizeof(int)*matrixWidth*matrixWidth);
    for(int i=0;i<matrixWidth*matrixWidth;i++){
        h_a[i]=1;
        h_b[i]=1;
        h_c[i]=0;
    }

    //threads (x,y,z) = (16,16)
    cldim3 threads(16,16); //equal to (16,16,1)
    //blocks (x,y,z) = (64,64)
    cldim3 blocks(matrixWidth/16,matrixWidth/16);  //equal to (64,64,1)

    //memory copy from host to device
    ts.stamp();
    clo.clMemcpy(d_a,h_a,sizeof(int)*matrixWidth*matrixWidth,clMemcpyHostToDevice);
    clo.clMemcpy(d_b,h_b,sizeof(int)*matrixWidth*matrixWidth,clMemcpyHostToDevice);
    //clo.clMemcpy(d_c,h_c,sizeof(int)*matrixWidth*matrixWidth,clMemcpyHostToDevice);
    ts.stamp();

    //run kernel *blocking(sync) execution
    cl_int err;
    err = clo.runkernel(kernel1,blocks,threads);

    ts.stamp();

    //memory copy from host to device
    clo.clMemcpy(h_c,d_c,sizeof(int)*matrixWidth*matrixWidth,clMemcpyDeviceToHost);

    ts.stamp();

    if(err!=CL_SUCCESS){
        printf("clEnqueueNDRangeKerne error %s\n",getErrorString(err));
        exit(EXIT_FAILURE);
    }

/*
    for(int i=0;i<matrixWidth*matrixWidth;i++){
        if(h_c[i]!=matrixWidth){
            printf("Error: %d %d\n",i,h_c[i]);
            break;
        }
    }
*/
//    ts.print();
    printf("--------------------------------------\n");
    printf("[ Matrix Multiplication(%d x %d) ]\n",matrixWidth,matrixWidth);
    printf("host->device, %6.3f,ms\n",ts.interval(0,1)*1000);
    printf("kernel      , %6.3f,ms\n",ts.interval(1,2)*1000);
    printf("device->host, %6.3f,ms\n",ts.interval(2,3)*1000);

    clReleaseMemObject(*d_a);
    clReleaseMemObject(*d_b);
    clReleaseMemObject(*d_c);

    return 0;
}
