#ifndef ENUM_H
#define ENUM_H

namespace ZH{
    namespace Graphics{

        enum DEVICE_FEATURE_LEVEL
        {
            DEVICE_FEATURE_LEVEL_9_1	= 0x9100,
            DEVICE_FEATURE_LEVEL_9_2	= 0x9200,
            DEVICE_FEATURE_LEVEL_9_3	= 0x9300,
            DEVICE_FEATURE_LEVEL_10_0	= 0xa000,
            DEVICE_FEATURE_LEVEL_10_1	= 0xa100,
            DEVICE_FEATURE_LEVEL_11_0	= 0xb000,
            DEVICE_FEATURE_LEVEL_11_1	= 0xb100
        };

        enum TEX2D_FORMAT {
            TEX2D_FORMAT_UNKNOWN                      = 0,
            TEX2D_FORMAT_R32G32B32A32_TYPELESS        = 1,
            TEX2D_FORMAT_R32G32B32A32_FLOAT           = 2,
            TEX2D_FORMAT_R32G32B32A32_UINT            = 3,
            TEX2D_FORMAT_R32G32B32A32_SINT            = 4,
            TEX2D_FORMAT_R32G32B32_TYPELESS           = 5,
            TEX2D_FORMAT_R32G32B32_FLOAT              = 6,
            TEX2D_FORMAT_R32G32B32_UINT               = 7,
            TEX2D_FORMAT_R32G32B32_SINT               = 8,
            TEX2D_FORMAT_R16G16B16A16_TYPELESS        = 9,
            TEX2D_FORMAT_R16G16B16A16_FLOAT           = 10,
            TEX2D_FORMAT_R16G16B16A16_UNORM           = 11,
            TEX2D_FORMAT_R16G16B16A16_UINT            = 12,
            TEX2D_FORMAT_R16G16B16A16_SNORM           = 13,
            TEX2D_FORMAT_R16G16B16A16_SINT            = 14,
            TEX2D_FORMAT_R32G32_TYPELESS              = 15,
            TEX2D_FORMAT_R32G32_FLOAT                 = 16,
            TEX2D_FORMAT_R32G32_UINT                  = 17,
            TEX2D_FORMAT_R32G32_SINT                  = 18,
            TEX2D_FORMAT_R32G8X24_TYPELESS            = 19,
            TEX2D_FORMAT_D32_FLOAT_S8X24_UINT         = 20,
            TEX2D_FORMAT_R32_FLOAT_X8X24_TYPELESS     = 21,
            TEX2D_FORMAT_X32_TYPELESS_G8X24_UINT      = 22,
            TEX2D_FORMAT_R10G10B10A2_TYPELESS         = 23,
            TEX2D_FORMAT_R10G10B10A2_UNORM            = 24,
            TEX2D_FORMAT_R10G10B10A2_UINT             = 25,
            TEX2D_FORMAT_R11G11B10_FLOAT              = 26,
            TEX2D_FORMAT_R8G8B8A8_TYPELESS            = 27,
            TEX2D_FORMAT_R8G8B8A8_UNORM               = 28,
            TEX2D_FORMAT_R8G8B8A8_UNORM_SRGB          = 29,
            TEX2D_FORMAT_R8G8B8A8_UINT                = 30,
            TEX2D_FORMAT_R8G8B8A8_SNORM               = 31,
            TEX2D_FORMAT_R8G8B8A8_SINT                = 32,
            TEX2D_FORMAT_R16G16_TYPELESS              = 33,
            TEX2D_FORMAT_R16G16_FLOAT                 = 34,
            TEX2D_FORMAT_R16G16_UNORM                 = 35,
            TEX2D_FORMAT_R16G16_UINT                  = 36,
            TEX2D_FORMAT_R16G16_SNORM                 = 37,
            TEX2D_FORMAT_R16G16_SINT                  = 38,
            TEX2D_FORMAT_R32_TYPELESS                 = 39,
            TEX2D_FORMAT_D32_FLOAT                    = 40,
            TEX2D_FORMAT_R32_FLOAT                    = 41,
            TEX2D_FORMAT_R32_UINT                     = 42,
            TEX2D_FORMAT_R32_SINT                     = 43,
            TEX2D_FORMAT_R24G8_TYPELESS               = 44,
            TEX2D_FORMAT_D24_UNORM_S8_UINT            = 45,
            TEX2D_FORMAT_R24_UNORM_X8_TYPELESS        = 46,
            TEX2D_FORMAT_X24_TYPELESS_G8_UINT         = 47,
            TEX2D_FORMAT_R8G8_TYPELESS                = 48,
            TEX2D_FORMAT_R8G8_UNORM                   = 49,
            TEX2D_FORMAT_R8G8_UINT                    = 50,
            TEX2D_FORMAT_R8G8_SNORM                   = 51,
            TEX2D_FORMAT_R8G8_SINT                    = 52,
            TEX2D_FORMAT_R16_TYPELESS                 = 53,
            TEX2D_FORMAT_R16_FLOAT                    = 54,
            TEX2D_FORMAT_D16_UNORM                    = 55,
            TEX2D_FORMAT_R16_UNORM                    = 56,
            TEX2D_FORMAT_R16_UINT                     = 57,
            TEX2D_FORMAT_R16_SNORM                    = 58,
            TEX2D_FORMAT_R16_SINT                     = 59,
            TEX2D_FORMAT_R8_TYPELESS                  = 60,
            TEX2D_FORMAT_R8_UNORM                     = 61,
            TEX2D_FORMAT_R8_UINT                      = 62,
            TEX2D_FORMAT_R8_SNORM                     = 63,
            TEX2D_FORMAT_R8_SINT                      = 64,
            TEX2D_FORMAT_A8_UNORM                     = 65,
            TEX2D_FORMAT_R1_UNORM                     = 66,
            TEX2D_FORMAT_R9G9B9E5_SHAREDEXP           = 67,
            TEX2D_FORMAT_R8G8_B8G8_UNORM              = 68,
            TEX2D_FORMAT_G8R8_G8B8_UNORM              = 69,
            TEX2D_FORMAT_BC1_TYPELESS                 = 70,
            TEX2D_FORMAT_BC1_UNORM                    = 71,
            TEX2D_FORMAT_BC1_UNORM_SRGB               = 72,
            TEX2D_FORMAT_BC2_TYPELESS                 = 73,
            TEX2D_FORMAT_BC2_UNORM                    = 74,
            TEX2D_FORMAT_BC2_UNORM_SRGB               = 75,
            TEX2D_FORMAT_BC3_TYPELESS                 = 76,
            TEX2D_FORMAT_BC3_UNORM                    = 77,
            TEX2D_FORMAT_BC3_UNORM_SRGB               = 78,
            TEX2D_FORMAT_BC4_TYPELESS                 = 79,
            TEX2D_FORMAT_BC4_UNORM                    = 80,
            TEX2D_FORMAT_BC4_SNORM                    = 81,
            TEX2D_FORMAT_BC5_TYPELESS                 = 82,
            TEX2D_FORMAT_BC5_UNORM                    = 83,
            TEX2D_FORMAT_BC5_SNORM                    = 84,
            TEX2D_FORMAT_B5G6R5_UNORM                 = 85,
            TEX2D_FORMAT_B5G5R5A1_UNORM               = 86,
            TEX2D_FORMAT_B8G8R8A8_UNORM               = 87,
            TEX2D_FORMAT_B8G8R8X8_UNORM               = 88,
            TEX2D_FORMAT_R10G10B10_XR_BIAS_A2_UNORM   = 89,
            TEX2D_FORMAT_B8G8R8A8_TYPELESS            = 90,
            TEX2D_FORMAT_B8G8R8A8_UNORM_SRGB          = 91,
            TEX2D_FORMAT_B8G8R8X8_TYPELESS            = 92,
            TEX2D_FORMAT_B8G8R8X8_UNORM_SRGB          = 93,
            TEX2D_FORMAT_BC6H_TYPELESS                = 94,
            TEX2D_FORMAT_BC6H_UF16                    = 95,
            TEX2D_FORMAT_BC6H_SF16                    = 96,
            TEX2D_FORMAT_BC7_TYPELESS                 = 97,
            TEX2D_FORMAT_BC7_UNORM                    = 98,
            TEX2D_FORMAT_BC7_UNORM_SRGB               = 99,
            TEX2D_FORMAT_FORCE_UINT                   = 0xffffffffUL 
        };

        enum USAGE {
            USAGE_DEFAULT     = 0,  // CPU:          , GPU: Read|Write
            USAGE_IMMUTABLE   = 1,  // CPU: No access, GPU: Read
            USAGE_DYNAMIC     = 2,  // CPU: Write,     GPU: Read
            USAGE_STAGING     = 3   // GPU  --> CPU copy
        };

        enum BIND_FLAG {
            BIND_VERTEX_BUFFER      = 0x1L,
            BIND_INDEX_BUFFER       = 0x2L,
            BIND_CONSTANT_BUFFER    = 0x4L,
            BIND_SHADER_RESOURCE    = 0x8L,
            BIND_STREAM_OUTPUT      = 0x10L,
            BIND_RENDER_TARGET      = 0x20L,
            BIND_DEPTH_STENCIL      = 0x40L,
            BIND_UNORDERED_ACCESS   = 0x80L 
        };

        enum CPU_ACCESS_FLAG {
            CPU_ACCESS_NO      = 0x0L,
            CPU_ACCESS_WRITE   = 0x10000L,
            CPU_ACCESS_READ    = 0x20000L 
        };

        enum RESOURCE_MISC_FLAG {
            RESOURCE_MISC_UNUSED                   = 0x0L,
            RESOURCE_MISC_GENERATE_MIPS            = 0x1L,
            RESOURCE_MISC_SHARED                   = 0x2L,
            RESOURCE_MISC_TEXTURECUBE              = 0x4L,
            RESOURCE_MISC_DRAWINDIRECT_ARGS        = 0x10L,
            RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS   = 0x20L,
            RESOURCE_MISC_BUFFER_STRUCTURED        = 0x40L,
            RESOURCE_MISC_RESOURCE_CLAMP           = 0x80L,
            RESOURCE_MISC_SHARED_KEYEDMUTEX        = 0x100L,
            RESOURCE_MISC_GDI_COMPATIBLE           = 0x200L 
        };

        enum PRIMITIVE_TYPE {
            PRIMITIVE_TOPOLOGY_UNDEFINED                    = 0,
            PRIMITIVE_TOPOLOGY_POINTLIST                    = 1,
            PRIMITIVE_TOPOLOGY_LINELIST                     = 2,
            PRIMITIVE_TOPOLOGY_LINESTRIP                    = 3,
            PRIMITIVE_TOPOLOGY_TRIANGLELIST                 = 4,
            PRIMITIVE_TOPOLOGY_TRIANGLESTRIP                = 5
        };




    } // namespace Graphics

} // namespace ZH

#endif

