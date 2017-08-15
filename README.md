# OpenCL Wrapper like CUDA

## Usage

### include
```
#include "my_clo.hpp"
```

### instatiation
```
clLikeCUDA clo;///platform_id = 0, device_id = 0
clLikeCUDA clo(deviceNumber);
clLikeCUDA clo(-1);///stdio selection mode
```

### build OpenCL source
```
cl_kernel kernel1 = clo.clCreateKernelFromFile(fileName, functionName, options);
```


### malloc device memory
```cl_mem_flag``` -> https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clCreateBuffer.html

```
cl_mem *d_a;
clo.clMalloc(&d_a,sizeof(int)*1024*1024,CL_MEM_READ_ONLY);
```

### set arguments
```
#if __cplusplus >= 201103L  //-std=c++11
    clo.clSetKernelArgs(kernel1, d_a, d_b);
#else
    clSetKernelArg(kernel1,0,sizeof(cl_mem),d_a);
    clSetKernelArg(kernel1,1,sizeof(cl_mem),d_b);
#endif
```

### memory copy
* ```clMemcpyHostToDevice```  device -> host
* ```clMemcpyDeviceToHost```  host -> device
* ```clMemcpyDeivceToDevice```  device -> device

```
clo.clMemcpy(d_a,h_a,sizeof(int)*1024*1024,clMemcpyHostToDevice);
```

### run kernel
```
cldim3 blocks(64,64);
cldim3 threads(16,16);

cl_int err = clo.runkernel(kernel1,blocks,threads);
```



## Compile
```
g++ -o clcheat clcheat.cpp -std=c++11 -lOpenCL
clang++ -o clcheat clcheat.cpp -std=c++11 -framework OpenCL
```
## Run
```
$./clcheat
[No.] platform = "platformName", device = "deviceName"
----------------------------------------------------------
[  0] platform = "NVIDIA CUDA", device = "GeForce GTX 750 Ti"
[  1] platform = "Intel(R) OpenCL", device = "Intel(R) Core(TM) i5-4440 CPU @ 3.10GHz"
----------------------------------------------------------
input device number : 0
----------------------------------------------------------
CL_PLATFORM_NAME,NVIDIA CUDA,CL_DEVICE_NAME,GeForce GTX 750 Ti
--------------------------------------
[ Matrix Multiplication(1024 x 1024) ]
host->device,  3.799,ms
kernel      , 12.602,ms
device->host,  0.859,ms

```
