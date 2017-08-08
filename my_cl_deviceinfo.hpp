#ifndef _CLDEVICEINFO
#define _CLDEVICEINFO




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

#define BUFFERSIZE 1024
#define PLATFORM_MAX_NUM 32
#define DEVICE_MAX_NUM 32

void PrintDeviceQuery();/*標準入力からデバイスを選択し表示*/

void PrintDeviceInfoALL();/*全てのプラットフォームの全てのデバイス情報表示*/
void PrintDeviceInfoCPU();/*全てのプラットフォームの全てのGPU情報表示*/
void PrintDeviceInfoGPU();/*全てのプラットフォームの全てのCPU情報表示*/

void PrintOnedeviceInfo(cl_device_id device_id);/*引数のデバイス情報表示*/
void PrintOneplatformInfo(cl_platform_id platform_id);/*引数のプラットフォーム情報表示*/
void PrintDeviceInfo();




void PrintDeviceQuery(){

    cl_platform_id platform_id[PLATFORM_MAX_NUM];
    cl_device_id device_id[DEVICE_MAX_NUM];
    cl_uint devnum;
    cl_uint num_platforms;
    cl_platform_id clSelectedPlatformID = NULL;

    char buffer[BUFFERSIZE];
    size_t size_ret;

    clGetPlatformIDs (PLATFORM_MAX_NUM, platform_id, &num_platforms);
    printf("The number of PlatForm              %d\n",num_platforms);
    int i,j;
    printf("Choose Platform\n");
    printf("%d : END PrintDeviceQuery\n",0);
    for(i=0;i<num_platforms;i++){
        clGetPlatformInfo(platform_id[i],CL_PLATFORM_NAME,sizeof(buffer),buffer,&size_ret);
        printf("%d : %s\n",i,buffer);

    }
    unsigned int input;
    int scanf_returnvalue = scanf("%d",&input);
    fflush(stdin);
    /*if(input==0){
        printf("END PrintDeviceQuery\n");
        return;
    }*/
    clGetPlatformInfo(platform_id[input],CL_PLATFORM_NAME,sizeof(buffer),buffer,&size_ret);
    printf("PLATFORM : %-40s\n",buffer);



    printf("Choose Device\n");
    //printf("%d : END PrintDeviceQuery\n",0);

    clGetDeviceIDs( platform_id[input], CL_DEVICE_TYPE_ALL, DEVICE_MAX_NUM, device_id, &devnum);

    for(i=0;i<devnum;i++){
        clGetDeviceInfo( device_id[i], CL_DEVICE_NAME  , sizeof(buffer), &buffer, &size_ret);
        printf("%d : %s\n",i,buffer);
    }
    scanf_returnvalue = scanf("%d",&input);
    fflush(stdin);
    /*if(input==0){
        printf("END PrintDeviceQuery\n");
        return;
    }*/
    clGetDeviceInfo( device_id[input], CL_DEVICE_NAME  , sizeof(buffer), &buffer, &size_ret);
    printf("DEVICE : %-40s\n",buffer);
    PrintOnedeviceInfo( device_id[input] );
}


void PrintDeviceInfoALL(){
    cl_platform_id platform_id[PLATFORM_MAX_NUM];
    cl_device_id device_id[DEVICE_MAX_NUM];
    cl_uint devnum;
    cl_uint num_platforms;
    cl_platform_id clSelectedPlatformID = NULL;
    clGetPlatformIDs (PLATFORM_MAX_NUM, platform_id, &num_platforms);
    printf("The number of PlatForm              %d\n",num_platforms);
    int i,j;
    for(i=0;i<num_platforms;i++){
        clGetDeviceIDs( platform_id[i], CL_DEVICE_TYPE_ALL, DEVICE_MAX_NUM, device_id, &devnum);
        printf("\n\n\n\nPLATFORM NO.%02d   The number of Alldevice  %d\n",i,devnum);
        PrintOneplatformInfo(platform_id[i]);
        for(j=0;j<devnum;j++){
            printf("+++++++++++++++++++++++DEVICE NO.%02d\n\n",j);
            PrintOnedeviceInfo(device_id[j]);
        }
    }
}

void PrintDeviceInfoGPU(){
    cl_platform_id platform_id[PLATFORM_MAX_NUM];
    cl_device_id device_id[DEVICE_MAX_NUM];
    cl_uint devnum;
    cl_uint num_platforms;
    cl_platform_id clSelectedPlatformID = NULL;
    clGetPlatformIDs (PLATFORM_MAX_NUM, platform_id, &num_platforms);
    printf("The number of PlatForm              %d\n",num_platforms);
    int i,j;
    for(i=0;i<num_platforms;i++){
        clGetDeviceIDs( platform_id[i], CL_DEVICE_TYPE_GPU, DEVICE_MAX_NUM, device_id, &devnum);
        printf("\n\n\n\nPLATFORM NO.%02d   The number of GPU  %d\n",i,devnum);
        PrintOneplatformInfo(platform_id[i]);
        for(j=0;j<devnum;j++){
            printf("+++++++++++++++++++++++DEVICE NO.%02d+++++++++++++++++++++++\n\n",j);
            PrintOnedeviceInfo(device_id[j]);
        }
    }
}

void PrintDeviceInfoCPU(){
    cl_platform_id platform_id[PLATFORM_MAX_NUM];
    cl_device_id device_id[DEVICE_MAX_NUM];
    cl_uint devnum;
    cl_uint num_platforms;
    cl_platform_id clSelectedPlatformID = NULL;
    clGetPlatformIDs (PLATFORM_MAX_NUM, platform_id, &num_platforms);
    printf("The number of PlatForm              %d\n",num_platforms);
    int i,j;
    for(i=0;i<num_platforms;i++){
        clGetDeviceIDs( platform_id[i], CL_DEVICE_TYPE_CPU, DEVICE_MAX_NUM, device_id, &devnum);
        printf("\n\n\n\nPLATFORM NO.%02d   The number of CPU  %d\n",i,devnum);
        PrintOneplatformInfo(platform_id[i]);
        for(j=0;j<devnum;j++){
            printf("+++++++++++++++++++++++DEVICE NO.%02d+++++++++++++++++++++++\n\n",j);
            PrintOnedeviceInfo(device_id[j]);
        }
    }
}

void PrintOneplatformInfo(cl_platform_id platform_id){
    char buffer[BUFFERSIZE];
    size_t size_ret;
    clGetPlatformInfo(platform_id,CL_PLATFORM_PROFILE,sizeof(buffer),buffer,&size_ret);
    printf("%-40s%s\n","CL_PLATFORM_PROFILE",buffer);
    clGetPlatformInfo(platform_id,CL_PLATFORM_VERSION,sizeof(buffer),buffer,&size_ret);
    printf("%-40s%s\n","CL_PLATFORM_VERSION",buffer);
    clGetPlatformInfo(platform_id,CL_PLATFORM_NAME,sizeof(buffer),buffer,&size_ret);
    printf("%-40s%s\n","CL_PLATFORM_NAME",buffer);
    clGetPlatformInfo(platform_id,CL_PLATFORM_VENDOR,sizeof(buffer),buffer,&size_ret);
    printf("%-40s%s\n","CL_PLATFORM_VENDOR",buffer);
    clGetPlatformInfo(platform_id,CL_PLATFORM_EXTENSIONS,sizeof(buffer),buffer,&size_ret);
    printf("%-40s%s\n","CL_PLATFORM_EXTENSIONS",buffer);
}

void PrintDeviceInfo(){

    char info[64][64]={
    "CL_DEVICE_TYPE_CPU",//
    "CL_DEVICE_TYPE_GPU",//
    "CL_DEVICE_TYPE_ACCELERATOR",//
    "CL_DEVICE_TYPE_DEFAULT",//
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",

    "CL_FP_DENORM",// - 非正規化数が利用できます
    "CL_FP_INF_NAN",// - INF および quiet NaN が利用できます
    "CL_FP_ROUND_TO_NEAREST",// - 最近接偶数丸めモードが利用できます
    "CL_FP_ROUND_TO_ZERO",// - 0方向丸めモードが利用できます
    "CL_FP_ROUND_TO_INF",// - 正無限大方向丸めモードおよび負無限大方向丸めモードが利用できます
    "CL_FP_FMA",// - IEEE 754-2008 で規定されている Fused Multiply-Add (FMA) が利用できます。
    "CL_FP_SOFT_FLOAT",// - 基本的な浮動小数点操作（加算、減算、乗算など）がソフトウェアで実装されています)
    "Unknown value",

    "CL_NONE",//
    "CL_READ_ONLY_CACHE",//
    "CL_READ_WRITE_CACHE",//
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",

    "CL_LOCA",//L - SRAMのような専用のローカルメモリが用意されていることを示唆しています
    "CL_GLOBAL",//
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",

    "CL_EXEC_KERNEL",// - OpenCLカーネルを実行できます
    "CL_EXEC_NATIVE_KERNEL",// - ネイティブカーネルを実行できます。
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",

    "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE",//
    "CL_QUEUE_PROFILING_ENABLE",//
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    };

    char buffer[BUFFERSIZE];
    cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	size_t size_ret,size;
    cl_uint devnum;
    cl_uint num_platforms;
	cl_platform_id clSelectedPlatformID = NULL;
	clGetPlatformIDs (1, &platform_id, &num_platforms);
    printf("The number of PlatForm              %d\n",num_platforms);
    clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &devnum);
	printf("The number of GPU              %d\n",devnum);

    size_t cls,clss[32];
    cl_uint cui;
    cl_ulong cul;
    cl_bool cbo;

    int i;

    clGetDeviceInfo( device_id, CL_DEVICE_NAME                           , sizeof(buffer), &buffer, &size_ret);
    printf("CL_DEVICE_NAME                               %s\n",buffer);

    clGetDeviceInfo( device_id, CL_DEVICE_VENDOR                         , sizeof(buffer), &buffer, &size_ret);
    printf("CL_DEVICE_VENDOR                             %s\n",buffer);

    clGetDeviceInfo( device_id, CL_DRIVER_VERSION                        , sizeof(buffer), &buffer, &size_ret);
    printf("CL_DRIVER_VERSION                            %s\n",buffer);

    clGetDeviceInfo( device_id, CL_DEVICE_PROFILE                        , sizeof(buffer), &buffer, &size_ret);
    printf("CL_DEVICE_PROFILE                            %s\n",buffer);

    clGetDeviceInfo( device_id, CL_DEVICE_VERSION                        , sizeof(buffer), &buffer, &size_ret);
    printf("CL_DEVICE_VERSION                            %s\n",buffer);

    clGetDeviceInfo( device_id, CL_DEVICE_OPENCL_C_VERSION               , sizeof(buffer), &buffer, &size_ret);
    printf("CL_DEVICE_OPENCL_C_VERSION                   %s\n",buffer);

    clGetDeviceInfo( device_id, CL_DEVICE_TYPE                           , sizeof(cul), &cul, &size_ret);
    printf("CL_DEVICE_TYPE                               | ");

    for(i=0;i<8;i++){
        if(cul&0x1){
            printf("%s | ",info[0+i]);
        }
       cul=cul>>1;
    }
   printf("\n");

    clGetDeviceInfo( device_id, CL_DEVICE_VENDOR_ID                      , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_VENDOR_ID                          %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_MAX_COMPUTE_UNITS              , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_MAX_COMPUTE_UNITS                  %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS       , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS           %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES            , sizeof(buffer), &clss, &size_ret);
    printf("CL_DEVICE_MAX_WORK_ITEM_SIZES                ");

    printf("(");
    for(i=0;i<cui-1;i++)
        printf("%zu,",clss[i]);
    printf("%zu)\n",clss[cui-1]);


    clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE            , sizeof(cls), &cls, &size_ret);
    printf("CL_DEVICE_MAX_WORK_GROUP_SIZE                %zu\n",cls);

    clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR    , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR        %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT   , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT       %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT     , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT         %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG    , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG        %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT   , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT       %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE  , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE      %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF    , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF        %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR       , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR           %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT      , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT          %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_INT        , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_INT            %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG       , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG           %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT      , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT          %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE     , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE         %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF       , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF           %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_MAX_CLOCK_FREQUENCY            , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_MAX_CLOCK_FREQUENCY                %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_ADDRESS_BITS                   , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_ADDRESS_BITS                       %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_MAX_MEM_ALLOC_SIZE             , sizeof(cul), &cul, &size_ret);
    printf("CL_DEVICE_MAX_MEM_ALLOC_SIZE                 %zu\n",cul);

    clGetDeviceInfo( device_id, CL_DEVICE_IMAGE_SUPPORT                  , sizeof(cbo), &cbo, &size_ret);
    printf("CL_DEVICE_IMAGE_SUPPORT                      ");
    if(cbo)
        printf("Yes \n");
    else
        printf("No \n");

    clGetDeviceInfo( device_id, CL_DEVICE_MAX_READ_IMAGE_ARGS            , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_MAX_READ_IMAGE_ARGS                %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_MAX_WRITE_IMAGE_ARGS           , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_MAX_WRITE_IMAGE_ARGS               %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_IMAGE2D_MAX_WIDTH              , sizeof(cls), &cls, &size_ret);
    printf("CL_DEVICE_IMAGE2D_MAX_WIDTH                  %zu\n",cls);

    clGetDeviceInfo( device_id, CL_DEVICE_IMAGE2D_MAX_HEIGHT             , sizeof(cls), &cls, &size_ret);
    printf("CL_DEVICE_IMAGE2D_MAX_HEIGHT                 %zu\n",cls);

    clGetDeviceInfo( device_id, CL_DEVICE_IMAGE3D_MAX_WIDTH              , sizeof(cls), &cls, &size_ret);
    printf("CL_DEVICE_IMAGE3D_MAX_WIDTH                  %zu\n",cls);

    clGetDeviceInfo( device_id, CL_DEVICE_IMAGE3D_MAX_HEIGHT             , sizeof(cls), &cls, &size_ret);
    printf("CL_DEVICE_IMAGE3D_MAX_HEIGHT                 %zu\n",cls);

    clGetDeviceInfo( device_id, CL_DEVICE_IMAGE3D_MAX_DEPTH              , sizeof(cls), &cls, &size_ret);
    printf("CL_DEVICE_IMAGE3D_MAX_DEPTH                  %zu\n",cls);

    clGetDeviceInfo( device_id, CL_DEVICE_MAX_SAMPLERS                   , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_MAX_SAMPLERS                       %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_MAX_PARAMETER_SIZE             , sizeof(cls), &cls, &size_ret);
    printf("CL_DEVICE_MAX_PARAMETER_SIZE                 %zu\n",cls);

    clGetDeviceInfo( device_id, CL_DEVICE_MEM_BASE_ADDR_ALIGN            , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_MEM_BASE_ADDR_ALIGN                %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE       , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE           %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_SINGLE_FP_CONFIG               , sizeof(cul), &cul, &size_ret);
    printf("CL_DEVICE_SINGLE_FP_CONFIG                   | ");

    for(i=0;i<8;i++){
        if(cul&0x1){
            printf("%s | ",info[8+i]);
        }
       cul=cul>>1;
    }
   printf("\n");

    clGetDeviceInfo( device_id, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE          , sizeof(cul), &cul, &size_ret);
    printf("CL_DEVICE_GLOBAL_MEM_CACHE_TYPE              | ");

    for(i=0;i<3;i++){
        if(cul&0x1){
            printf("%s | ",info[16+i]);
        }
       cul=cul>>1;
    }
   printf("\n");

    clGetDeviceInfo( device_id, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE      , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE          %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE          , sizeof(cul), &cul, &size_ret);
    printf("CL_DEVICE_GLOBAL_MEM_CACHE_SIZE              %zu\n",cul);

    clGetDeviceInfo( device_id, CL_DEVICE_GLOBAL_MEM_SIZE                , sizeof(cul), &cul, &size_ret);
    printf("CL_DEVICE_GLOBAL_MEM_SIZE                    %zu\n",cul);

    clGetDeviceInfo( device_id, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE       , sizeof(cul), &cul, &size_ret);
    printf("CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE           %zu\n",cul);

    clGetDeviceInfo( device_id, CL_DEVICE_MAX_CONSTANT_ARGS              , sizeof(cui), &cui, &size_ret);
    printf("CL_DEVICE_MAX_CONSTANT_ARGS                  %u\n",cui);

    clGetDeviceInfo( device_id, CL_DEVICE_LOCAL_MEM_TYPE                 , sizeof(cul), &cul, &size_ret);
    printf("CL_DEVICE_LOCAL_MEM_TYPE                     | ");

    for(i=0;i<8;i++){
        if(cul&0x1){
            printf("%s | ",info[24+i]);
        }
       cul=cul>>1;
    }
   printf("\n");


    clGetDeviceInfo( device_id, CL_DEVICE_LOCAL_MEM_SIZE                 , sizeof(cul), &cul, &size_ret);
    printf("CL_DEVICE_LOCAL_MEM_SIZE                     %zu\n",cul);

    clGetDeviceInfo( device_id, CL_DEVICE_ERROR_CORRECTION_SUPPORT       , sizeof(cbo), &cbo, &size_ret);
    printf("CL_DEVICE_ERROR_CORRECTION_SUPPORT           ");
    if(cbo)
        printf("Yes \n");
    else
        printf("No \n");

    clGetDeviceInfo( device_id, CL_DEVICE_HOST_UNIFIED_MEMORY            , sizeof(cbo), &cbo, &size_ret);
    printf("CL_DEVICE_HOST_UNIFIED_MEMORY                ");
    if(cbo)
        printf("Yes \n");
    else
        printf("No \n");

    clGetDeviceInfo( device_id, CL_DEVICE_PROFILING_TIMER_RESOLUTION     , sizeof(cls), &cls, &size_ret);
    printf("CL_DEVICE_PROFILING_TIMER_RESOLUTION         %zu\n",cls);

    clGetDeviceInfo( device_id, CL_DEVICE_ENDIAN_LITTLE                  , sizeof(cbo), &cbo, &size_ret);
    printf("CL_DEVICE_ENDIAN_LITTLE                      ");
    if(cbo)
        printf("Yes \n");
    else
        printf("No \n");

    clGetDeviceInfo( device_id, CL_DEVICE_AVAILABLE                      , sizeof(cbo), &cbo, &size_ret);
    printf("CL_DEVICE_AVAILABLE                          ");
    if(cbo)
        printf("Yes \n");
    else
        printf("No \n");

    clGetDeviceInfo( device_id, CL_DEVICE_COMPILER_AVAILABLE             , sizeof(cbo), &cbo, &size_ret);
    printf("CL_DEVICE_COMPILER_AVAILABLE                 ");
    if(cbo)
        printf("Yes \n");
    else
        printf("No \n");

    clGetDeviceInfo( device_id, CL_DEVICE_EXECUTION_CAPABILITIES         , sizeof(cul), &cul, &size_ret);
    printf("CL_DEVICE_EXECUTION_CAPABILITIES             | ");

    for(i=0;i<8;i++){
        if(cul&0x1){
            printf("%s | ",info[32+i]);
        }
       cul=cul>>1;
    }
   printf("\n");

    clGetDeviceInfo( device_id, CL_DEVICE_QUEUE_PROPERTIES               , sizeof(cul), &cul, &size_ret);
    printf("CL_DEVICE_QUEUE_PROPERTIES                   | ");

     for(i=0;i<8;i++){
         if(cul&0x1){
             printf("%s | ",info[40+i]);
         }
        cul=cul>>1;
     }
    printf("\n");
    // clGetDeviceInfo( device_id, CL_DEVICE_PLATFORM                       , sizeof(cul), &cul, &size_ret);
    // printf("CL_DEVICE_PLATFORM                           %zu\n",cul);

    clGetDeviceInfo( device_id, CL_DEVICE_EXTENSIONS                     , sizeof(buffer), &buffer, &size_ret);
    printf("CL_DEVICE_EXTENSIONS                         %s\n",buffer);


}

void PrintOnedeviceInfo(cl_device_id device_id){


        char info[64][64]={
    "CL_DEVICE_TYPE_CPU",//
    "CL_DEVICE_TYPE_GPU",//
    "CL_DEVICE_TYPE_ACCELERATOR",//
    "CL_DEVICE_TYPE_DEFAULT",//
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",

    "CL_FP_DENORM",// - 非正規化数が利用できます
    "CL_FP_INF_NAN",// - INF および quiet NaN が利用できます
    "CL_FP_ROUND_TO_NEAREST",// - 最近接偶数丸めモードが利用できます
    "CL_FP_ROUND_TO_ZERO",// - 0方向丸めモードが利用できます
    "CL_FP_ROUND_TO_INF",// - 正無限大方向丸めモードおよび負無限大方向丸めモードが利用できます
    "CL_FP_FMA",// - IEEE 754-2008 で規定されている Fused Multiply-Add (FMA) が利用できます。
    "CL_FP_SOFT_FLOAT",// - 基本的な浮動小数点操作（加算、減算、乗算など）がソフトウェアで実装されています)
    "Unknown value",

    "CL_NONE",//
    "CL_READ_ONLY_CACHE",//
    "CL_READ_WRITE_CACHE",//
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",

    "CL_LOCA",//L - SRAMのような専用のローカルメモリが用意されていることを示唆しています
    "CL_GLOBAL",//
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",

    "CL_EXEC_KERNEL",// - OpenCLカーネルを実行できます
    "CL_EXEC_NATIVE_KERNEL",// - ネイティブカーネルを実行できます。
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",

    "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE",//
    "CL_QUEUE_PROFILING_ENABLE",//
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    "Unknown value",
    };

        char buffer[BUFFERSIZE];
    	size_t size_ret,size;
        cl_uint devnum;

        size_t cls,clss[32];
        cl_uint cui;
        cl_ulong cul;
        cl_bool cbo;

        int i;

        clGetDeviceInfo( device_id, CL_DEVICE_NAME                           , sizeof(buffer), &buffer, &size_ret);
        printf("CL_DEVICE_NAME                               %s\n",buffer);

        clGetDeviceInfo( device_id, CL_DEVICE_VENDOR                         , sizeof(buffer), &buffer, &size_ret);
        printf("CL_DEVICE_VENDOR                             %s\n",buffer);

        clGetDeviceInfo( device_id, CL_DRIVER_VERSION                        , sizeof(buffer), &buffer, &size_ret);
        printf("CL_DRIVER_VERSION                            %s\n",buffer);

        clGetDeviceInfo( device_id, CL_DEVICE_PROFILE                        , sizeof(buffer), &buffer, &size_ret);
        printf("CL_DEVICE_PROFILE                            %s\n",buffer);

        clGetDeviceInfo( device_id, CL_DEVICE_VERSION                        , sizeof(buffer), &buffer, &size_ret);
        printf("CL_DEVICE_VERSION                            %s\n",buffer);

        clGetDeviceInfo( device_id, CL_DEVICE_OPENCL_C_VERSION               , sizeof(buffer), &buffer, &size_ret);
        printf("CL_DEVICE_OPENCL_C_VERSION                   %s\n",buffer);

        clGetDeviceInfo( device_id, CL_DEVICE_TYPE                           , sizeof(cul), &cul, &size_ret);
        printf("CL_DEVICE_TYPE                               | ");

        for(i=0;i<8;i++){
            if(cul&0x1){
                printf("%s | ",info[0+i]);
            }
           cul=cul>>1;
        }
       printf("\n");

        clGetDeviceInfo( device_id, CL_DEVICE_VENDOR_ID                      , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_VENDOR_ID                          %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_MAX_COMPUTE_UNITS              , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_MAX_COMPUTE_UNITS                  %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS       , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS           %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES            , sizeof(buffer), &clss, &size_ret);
        printf("CL_DEVICE_MAX_WORK_ITEM_SIZES                ");

        printf("(");
        for(i=0;i<cui-1;i++)
            printf("%zu,",clss[i]);
        printf("%zu)\n",clss[cui-1]);


        clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE            , sizeof(cls), &cls, &size_ret);
        printf("CL_DEVICE_MAX_WORK_GROUP_SIZE                %zu\n",cls);

        clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR    , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR        %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT   , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT       %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT     , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT         %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG    , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG        %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT   , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT       %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE  , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE      %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF    , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF        %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR       , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR           %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT      , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT          %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_INT        , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_INT            %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG       , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG           %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT      , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT          %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE     , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE         %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF       , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF           %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_MAX_CLOCK_FREQUENCY            , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_MAX_CLOCK_FREQUENCY                %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_ADDRESS_BITS                   , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_ADDRESS_BITS                       %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_MAX_MEM_ALLOC_SIZE             , sizeof(cul), &cul, &size_ret);
        printf("CL_DEVICE_MAX_MEM_ALLOC_SIZE                 %zu\n",cul);

        clGetDeviceInfo( device_id, CL_DEVICE_IMAGE_SUPPORT                  , sizeof(cbo), &cbo, &size_ret);
        printf("CL_DEVICE_IMAGE_SUPPORT                      ");
        if(cbo)
            printf("Yes \n");
        else
            printf("No \n");

        clGetDeviceInfo( device_id, CL_DEVICE_MAX_READ_IMAGE_ARGS            , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_MAX_READ_IMAGE_ARGS                %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_MAX_WRITE_IMAGE_ARGS           , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_MAX_WRITE_IMAGE_ARGS               %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_IMAGE2D_MAX_WIDTH              , sizeof(cls), &cls, &size_ret);
        printf("CL_DEVICE_IMAGE2D_MAX_WIDTH                  %zu\n",cls);

        clGetDeviceInfo( device_id, CL_DEVICE_IMAGE2D_MAX_HEIGHT             , sizeof(cls), &cls, &size_ret);
        printf("CL_DEVICE_IMAGE2D_MAX_HEIGHT                 %zu\n",cls);

        clGetDeviceInfo( device_id, CL_DEVICE_IMAGE3D_MAX_WIDTH              , sizeof(cls), &cls, &size_ret);
        printf("CL_DEVICE_IMAGE3D_MAX_WIDTH                  %zu\n",cls);

        clGetDeviceInfo( device_id, CL_DEVICE_IMAGE3D_MAX_HEIGHT             , sizeof(cls), &cls, &size_ret);
        printf("CL_DEVICE_IMAGE3D_MAX_HEIGHT                 %zu\n",cls);

        clGetDeviceInfo( device_id, CL_DEVICE_IMAGE3D_MAX_DEPTH              , sizeof(cls), &cls, &size_ret);
        printf("CL_DEVICE_IMAGE3D_MAX_DEPTH                  %zu\n",cls);

        clGetDeviceInfo( device_id, CL_DEVICE_MAX_SAMPLERS                   , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_MAX_SAMPLERS                       %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_MAX_PARAMETER_SIZE             , sizeof(cls), &cls, &size_ret);
        printf("CL_DEVICE_MAX_PARAMETER_SIZE                 %zu\n",cls);

        clGetDeviceInfo( device_id, CL_DEVICE_MEM_BASE_ADDR_ALIGN            , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_MEM_BASE_ADDR_ALIGN                %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE       , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE           %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_SINGLE_FP_CONFIG               , sizeof(cul), &cul, &size_ret);
        printf("CL_DEVICE_SINGLE_FP_CONFIG                   | ");

        for(i=0;i<8;i++){
            if(cul&0x1){
                printf("%s | ",info[8+i]);
            }
           cul=cul>>1;
        }
       printf("\n");

        clGetDeviceInfo( device_id, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE          , sizeof(cul), &cul, &size_ret);
        printf("CL_DEVICE_GLOBAL_MEM_CACHE_TYPE              | ");

        for(i=0;i<3;i++){
            if(cul&0x1){
                printf("%s | ",info[16+i]);
            }
           cul=cul>>1;
        }
       printf("\n");

        clGetDeviceInfo( device_id, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE      , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE          %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE          , sizeof(cul), &cul, &size_ret);
        printf("CL_DEVICE_GLOBAL_MEM_CACHE_SIZE              %zu\n",cul);

        clGetDeviceInfo( device_id, CL_DEVICE_GLOBAL_MEM_SIZE                , sizeof(cul), &cul, &size_ret);
        printf("CL_DEVICE_GLOBAL_MEM_SIZE                    %zu\n",cul);

        clGetDeviceInfo( device_id, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE       , sizeof(cul), &cul, &size_ret);
        printf("CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE           %zu\n",cul);

        clGetDeviceInfo( device_id, CL_DEVICE_MAX_CONSTANT_ARGS              , sizeof(cui), &cui, &size_ret);
        printf("CL_DEVICE_MAX_CONSTANT_ARGS                  %u\n",cui);

        clGetDeviceInfo( device_id, CL_DEVICE_LOCAL_MEM_TYPE                 , sizeof(cul), &cul, &size_ret);
        printf("CL_DEVICE_LOCAL_MEM_TYPE                     | ");

        for(i=0;i<8;i++){
            if(cul&0x1){
                printf("%s | ",info[24+i]);
            }
           cul=cul>>1;
        }
       printf("\n");


        clGetDeviceInfo( device_id, CL_DEVICE_LOCAL_MEM_SIZE                 , sizeof(cul), &cul, &size_ret);
        printf("CL_DEVICE_LOCAL_MEM_SIZE                     %zu\n",cul);

        clGetDeviceInfo( device_id, CL_DEVICE_ERROR_CORRECTION_SUPPORT       , sizeof(cbo), &cbo, &size_ret);
        printf("CL_DEVICE_ERROR_CORRECTION_SUPPORT           ");
        if(cbo)
            printf("Yes \n");
        else
            printf("No \n");

        clGetDeviceInfo( device_id, CL_DEVICE_HOST_UNIFIED_MEMORY            , sizeof(cbo), &cbo, &size_ret);
        printf("CL_DEVICE_HOST_UNIFIED_MEMORY                ");
        if(cbo)
            printf("Yes \n");
        else
            printf("No \n");

        clGetDeviceInfo( device_id, CL_DEVICE_PROFILING_TIMER_RESOLUTION     , sizeof(cls), &cls, &size_ret);
        printf("CL_DEVICE_PROFILING_TIMER_RESOLUTION         %zu\n",cls);

        clGetDeviceInfo( device_id, CL_DEVICE_ENDIAN_LITTLE                  , sizeof(cbo), &cbo, &size_ret);
        printf("CL_DEVICE_ENDIAN_LITTLE                      ");
        if(cbo)
            printf("Yes \n");
        else
            printf("No \n");

        clGetDeviceInfo( device_id, CL_DEVICE_AVAILABLE                      , sizeof(cbo), &cbo, &size_ret);
        printf("CL_DEVICE_AVAILABLE                          ");
        if(cbo)
            printf("Yes \n");
        else
            printf("No \n");

        clGetDeviceInfo( device_id, CL_DEVICE_COMPILER_AVAILABLE             , sizeof(cbo), &cbo, &size_ret);
        printf("CL_DEVICE_COMPILER_AVAILABLE                 ");
        if(cbo)
            printf("Yes \n");
        else
            printf("No \n");

        clGetDeviceInfo( device_id, CL_DEVICE_EXECUTION_CAPABILITIES         , sizeof(cul), &cul, &size_ret);
        printf("CL_DEVICE_EXECUTION_CAPABILITIES             | ");

        for(i=0;i<8;i++){
            if(cul&0x1){
                printf("%s | ",info[32+i]);
            }
           cul=cul>>1;
        }
       printf("\n");

        clGetDeviceInfo( device_id, CL_DEVICE_QUEUE_PROPERTIES               , sizeof(cul), &cul, &size_ret);
        printf("CL_DEVICE_QUEUE_PROPERTIES                   | ");

         for(i=0;i<8;i++){
             if(cul&0x1){
                 printf("%s | ",info[40+i]);
             }
            cul=cul>>1;
         }
        printf("\n");
        // clGetDeviceInfo( device_id, CL_DEVICE_PLATFORM                       , sizeof(cul), &cul, &size_ret);
        // printf("CL_DEVICE_PLATFORM                           %zu\n",cul);

        clGetDeviceInfo( device_id, CL_DEVICE_EXTENSIONS                     , sizeof(buffer), &buffer, &size_ret);
        printf("CL_DEVICE_EXTENSIONS                         %s\n",buffer);


}




#endif
