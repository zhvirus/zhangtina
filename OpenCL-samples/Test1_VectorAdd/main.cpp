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
    cl_uint num_platforms = 10;
    cl_platform_id platforms[10];
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