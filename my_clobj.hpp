#ifndef _CLOBJ
#define _CLOBJ




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>
#include <fstream>
#include "my_cl_deviceinfo.hpp"
#include "my_cl_util.hpp"

enum clMemcpyKind{
    clMemcpyHostToDevice,
    clMemcpyDeviceToHost,
    clMemcpyDeviceToDevice,
};

class cldim3{
public:
    size_t d[3];
    cldim3(size_t x)                      {d[0] = x; d[1] = 1; d[2] = 1;}  ;
    cldim3(size_t x, size_t y)            {d[0] = x; d[1] = y; d[2] = 1;}  ;
    cldim3(size_t x, size_t y, size_t z)  {d[0] = x; d[1] = y; d[2] = z;}  ;
};

class clobj{
public:
    clobj();
    clobj(int deviceNumber);
    ~clobj();
    cl_kernel clCreateKernelFromFile(char *filename, char *funcname, char *options);
    void clMalloc(cl_mem **memadder, size_t size, cl_mem_flags flags);
    void clMemcpy(void * dst, const void *src, size_t size, enum clMemcpyKind kind);
    void clMemcpy(void * dst, const void *src, size_t dstoffset,size_t srcoffset, size_t size, enum clMemcpyKind kind);
    void Finish(){clFinish(queue);};
    void waitLastEvent(){clWaitForEvents(1, &event);};
    void printTargetInfo();
    void printTargetInfoDetail();
    cl_int runkernel(cl_kernel kernel, cldim3 &blocks, cldim3 &threads);
    cl_int runkernel(cl_kernel kernel, size_t blocks, size_t threads);

    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_event event;


#if __cplusplus >= 201103L  //-std=c++11
public:
    template <typename ... Args>
    void clSetKernelArgs(cl_kernel kernel, Args* ... args){
        clSetKernelArg_local(0, kernel, args...);
    };
private:
    template <typename T>
    void clSetKernelArg_local(int counter,  cl_kernel kernel, T* a){
        clSetKernelArg(kernel,counter,sizeof(*a),(const void *)a);
    };
    template <typename Head, typename  ...Tail>
    void clSetKernelArg_local(int counter,  cl_kernel kernel, Head* a, Tail* ... tail){
        clSetKernelArg(kernel,counter,sizeof(*a),(const void *)a);
        clSetKernelArg_local(counter+1,kernel, tail...);
    };
#endif
};
clobj::clobj(){
    cl_int err;
    err=clGetPlatformIDs(1,&platform,NULL);
    if(err != CL_SUCCESS){
        fprintf(stderr,"clGetPlatformIDs %d\n",err);
        exit(EXIT_FAILURE);
    }
    err=clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,1,&device,NULL);
    if(err != CL_SUCCESS){
        fprintf(stderr,"clGetDeviceIDs error %d\n",err);
        exit(EXIT_FAILURE);
    }
    //PrintTarget();
    context=clCreateContext(NULL,1,&device,NULL,NULL,&err);
    if(err != CL_SUCCESS){
        fprintf(stderr,"clCreateContext error %d\n",err);
        exit(EXIT_FAILURE);
    }
    queue = clCreateCommandQueue/*WithProperties*/(context,device,0,&err);


}
clobj::clobj(int deviceNumber){
    cl_int err;

    cl_platform_id platform_id[PLATFORM_MAX_NUM];
    cl_device_id device_id[DEVICE_MAX_NUM];
    cl_uint devnum;
    cl_uint num_platforms;
    cl_platform_id clSelectedPlatformID = NULL;
    char buffer[1024];
    size_t size_ret;
    int counter = 0;
    clGetPlatformIDs (PLATFORM_MAX_NUM, platform_id, &num_platforms);
    std::vector< std::pair<cl_platform_id, cl_device_id> > plat_dev_pairs;

    for(int i=0;i<num_platforms;i++){
        clGetDeviceIDs( platform_id[i], CL_DEVICE_TYPE_ALL, DEVICE_MAX_NUM, device_id, &devnum);

        clGetPlatformInfo(platform_id[i],CL_PLATFORM_NAME,sizeof(buffer),buffer,&size_ret);
        if(deviceNumber==-1) printf("%s\n",buffer,i);

        for(int j=0;j<devnum;j++){
            clGetDeviceInfo( device_id[j], CL_DEVICE_NAME, sizeof(buffer), &buffer, &size_ret);
        	if(deviceNumber==-1) printf("  <%d-%d> %s [%d]\n",i,j,buffer,counter);
            counter++;
            plat_dev_pairs.push_back( std::make_pair( platform_id[i] , device_id[j] ) );
        }
    }


    if(deviceNumber==-1){
        scanf("%d",&deviceNumber);
        fflush(stdin);
    }
    if(deviceNumber<counter && deviceNumber >= 0){
        device = plat_dev_pairs[deviceNumber].second;
    }else{
        device = 0;
    }
    context=clCreateContext(NULL,1,&device,NULL,NULL,&err);
    if(err != CL_SUCCESS){
        fprintf(stderr,"clCreateContext error %d\n",err);
        exit(EXIT_FAILURE);
    }
    queue = clCreateCommandQueue/*WithProperties*/(context,device,0,&err);
    if(err != CL_SUCCESS){
        fprintf(stderr,"clCreateCommandQueue error %d\n",err);
        exit(EXIT_FAILURE);
    }
}
clobj::~clobj(){
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}
void clobj::printTargetInfo(){
    char buffer[1024];
    size_t size_ret;
    clGetPlatformInfo(platform,CL_PLATFORM_NAME,sizeof(buffer),buffer,&size_ret);
    printf("%s,%s,","CL_PLATFORM_NAME",buffer);
    clGetDeviceInfo( device, CL_DEVICE_NAME, sizeof(buffer), &buffer, &size_ret);
	printf("%s,%s\n","CL_DEVICE_NAME",buffer);
	// clGetDeviceInfo( device, CL_DEVICE_OPENCL_C_VERSION, sizeof(buffer), &buffer, &size_ret);
	// printf("CL_DEVICE_OPENCL_C_VERSION     %s\n",buffer);
}
void clobj::printTargetInfoDetail(){
    PrintOneplatformInfo(platform);/*引数のプラットフォーム情報表示*/
    PrintOnedeviceInfo(device);/*引数のデバイス情報表示*/
}
cl_kernel clobj::clCreateKernelFromFile(char *filename, char *funcname, char *options){
    cl_int err;
    std::ifstream ifs(filename);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
    const char *program_buffer = content.c_str();
    size_t program_size = content.length();
    cl_program program=clCreateProgramWithSource(context,1,&program_buffer,&program_size,&err);
    if (err != CL_SUCCESS){
        fprintf(stderr,"clCreateProgramWithSource error: %d",err);
        exit(EXIT_FAILURE);
    }
    err = clBuildProgram(program, 1, &device, options, NULL, NULL);
    if (err != CL_SUCCESS){
        fprintf(stderr,"clBuildProgram error: %d",err);
        char *buff_erro;
        cl_int errcode;
        size_t build_log_len;
        errcode = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &build_log_len);
        buff_erro = (char *)malloc(build_log_len);
        errcode = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, build_log_len, buff_erro, NULL);
        fprintf(stderr,"%s\n",buff_erro);
        exit(EXIT_FAILURE);
    }
    cl_kernel kernel=clCreateKernel(program,funcname,&err);//CL_MEM_READ_ONLY,CL_MEM_WRITE_ONLY,CL_MEM_READ_WRITE
    if(err!=CL_SUCCESS){
        printf("clCreateKernel error %s\n",getErrorString(err));
        exit(EXIT_FAILURE);
    }
    clReleaseProgram(program);
    return kernel;
}
/*
void clobj::clMalloc(cl_mem *memadder, size_t size, cl_mem_flags flags){
    cl_int err;
    *memadder = clCreateBuffer(context,flags,size,NULL,&err);
    if(err!=CL_SUCCESS){
        printf("clCreateBuffer error %s\n",getErrorString(err));
        exit(EXIT_FAILURE);
    }
}
*/
void clobj::clMalloc(cl_mem **memadder, size_t size, cl_mem_flags flags){
    cl_int err;
    *memadder = (cl_mem*)malloc(sizeof(cl_mem));
    **memadder = clCreateBuffer(context,flags,size,NULL,&err);
    if(err!=CL_SUCCESS){
        printf("clCreateBuffer error %s\n",getErrorString(err));
        exit(EXIT_FAILURE);
    }
}
void clobj::clMemcpy(void * dst, const void *src, size_t dstoffset,size_t srcoffset, size_t size, enum clMemcpyKind kind){
    cl_int err;
    if(kind == clMemcpyHostToDevice){
        err =clEnqueueWriteBuffer(queue, *((cl_mem *)dst), CL_TRUE, srcoffset, size, src, 0, NULL, &event);
        if(err!=CL_SUCCESS){
            printf("clEnqueueWriteBuffer error %s\n",getErrorString(err));
            exit(EXIT_FAILURE);
        }
    }else if(kind == clMemcpyDeviceToHost){
        err =clEnqueueReadBuffer(queue, *((cl_mem *)src), CL_TRUE, dstoffset, size, dst, 0, NULL, &event);
        if(err!=CL_SUCCESS){
            printf("clEnqueueReadBuffer error %s\n",getErrorString(err));
            exit(EXIT_FAILURE);
        }
    }else if(kind == clMemcpyDeviceToDevice){
        err =clEnqueueCopyBuffer(queue, *((cl_mem *)src), *((cl_mem *)dst), srcoffset, dstoffset, size, 0, NULL, &event);
        if(err!=CL_SUCCESS){
            printf("clEnqueueCopyBuffer error %s\n",getErrorString(err));
            exit(EXIT_FAILURE);
        }
    }
}
void clobj::clMemcpy(void * dst, const void *src, size_t size, enum clMemcpyKind kind){
    clMemcpy(dst, src, 0, 0, size, kind);
}

cl_int clobj::runkernel(cl_kernel kernel, cldim3 &blocks, cldim3 &threads){
     size_t threads_per_grids[3];
    for(int i=0;i<3;i++) threads_per_grids[i] = blocks.d[i]*threads.d[i];
    cl_int err = clEnqueueNDRangeKernel(queue,kernel,3,NULL, threads_per_grids,threads.d,0,NULL,&event);
    clWaitForEvents(1, &event);
    return err;
}

cl_int clobj::runkernel(cl_kernel kernel, size_t blocks, size_t threads){
    size_t threads_per_grids = blocks*threads;
    cl_int err = clEnqueueNDRangeKernel(queue,kernel,1,NULL, &threads_per_grids, &threads,0,NULL,&event);
    clWaitForEvents(1, &event);
    return err;
}


#endif
