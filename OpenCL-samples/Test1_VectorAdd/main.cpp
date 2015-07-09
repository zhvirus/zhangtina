#include <CL/opencl.h>
#include <iostream>

struct platformInfo
{
    char PROFILE[100];
    char VERSION[100];
    char NAME[100];
    char VENDOR[100];
    char EXT[100];
    char ICD_SUFFIX[100];
};

void main()
{
    // Get platforms
    cl_platform_id platforms[10];
    {
        cl_uint num_platforms = 10;

        cl_uint ret_num_platforms = 0;
        if (CL_SUCCESS == clGetPlatformIDs(num_platforms, platforms, &ret_num_platforms)){
            std::cout << "Get platform ok!\n";
        }

        // Print platform info
        platformInfo* platInfs = new platformInfo[ret_num_platforms];
        for (cl_uint i = 0; i < ret_num_platforms; ++i){
            cl_uint ret_size = 0;
            clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 100, &(platInfs[i].PROFILE), &ret_size);
            clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 100, &(platInfs[i].VERSION), &ret_size);
            clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 100, &(platInfs[i].NAME), &ret_size);
            clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 100, &(platInfs[i].VENDOR), &ret_size);
            clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 100, &(platInfs[i].EXT), &ret_size);
            clGetPlatformInfo(platforms[i], CL_PLATFORM_ICD_SUFFIX_KHR, 100, &(platInfs[i].ICD_SUFFIX), &ret_size);
            std::cout << i << ": " << std::endl
                << "PROFILE: " << platInfs[i].PROFILE << std::endl
                << "VERSION: " << platInfs[i].VERSION << std::endl
                << "NAME: " << platInfs[i].NAME << std::endl
                << "VENDOR: " << platInfs[i].VENDOR << std::endl
                << "EXTENSIONS: " << platInfs[i].EXT << std::endl
                << "ICD_SUFFIX: " << platInfs[i].ICD_SUFFIX << std::endl
                ;
        }
    }

    // device
    cl_device_id device;
    {
        cl_uint num;
        clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 1, &device, &num);
    }

    // context
    cl_context context = clCreateContext(NULL,
        1,
        &device,
        NULL, NULL, NULL);

    //  command queue
    cl_command_queue queue = clCreateCommandQueueWithProperties(context,
        device,
        NULL, NULL);

    const unsigned int size = 10000;
    float* A = new float[size];
    float* B = new float[size];
    float* C = new float[size];
    for (unsigned int i = 0; i < size; ++i){
        A[i] = (float)(i+1);
        B[i] = (float)(i+1);
        C[i] = 8;
    }

    cl_mem buf1 = clCreateBuffer(
        context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*size, A, NULL);
    cl_mem buf2 = clCreateBuffer(
        context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*size, B, NULL);
    cl_mem buf3 = clCreateBuffer(
        context, CL_MEM_WRITE_ONLY, sizeof(float)*size, NULL, NULL);

    const char* source =
        "__kernel void vector_add(\n"
        "       __global const float* a,\n"
        "       __global const float* b,\n"
        "       __global float* c,\n"
        "       const unsigned int count)\n"
        "{\n"
        "   const int idx = get_global_id(0);\n"
        "   if (idx < count){\n"
        "        c[idx] = a[idx] + b[idx];\n"
        "    }\n"
        "}\n";
    const size_t ss = strlen(source);

    cl_program program = clCreateProgramWithSource(
        context,
        1,
        &source,
        &ss,
        NULL);

    cl_int status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (status != 0)
    {
        printf("clBuild failed:%d\n", status);
        char tbuf[0x10000];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0x10000, tbuf, NULL);
        printf("\n%s\n", tbuf);
        return;
    }
    else
    {
        printf("program build successful!\n");
    }

    // create kernel
    cl_kernel kernel = clCreateKernel(program, "vector_add", NULL);
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&buf1);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&buf2);
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&buf3);
    status = clSetKernelArg(kernel, 3, sizeof(unsigned int), (void*)&size);

    // launch kernel
    cl_event ev;
    size_t global_work_size[] = {100};
    status = clEnqueueNDRangeKernel(
        queue, kernel,
        1,
        NULL,
        &global_work_size[0],
        NULL,
        0, NULL, &ev
        );

    clEnqueueReadBuffer(
        queue, buf3,
        true,
        0, sizeof(float)*size,
        (void*)C,
        1,
        &ev,
        NULL
        );

    clFinish(queue);



}