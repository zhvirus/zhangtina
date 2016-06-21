#include <iostream>

#include "Widget\Window.h"
#include "Graphics\Device.h"
#include "d3d11.h"
#include "clew\clew.h"


ZH::Widgets::Window* pWin = NULL;

ID3D11Device* pDevice;
D3D_FEATURE_LEVEL featureLevel;
ID3D11DeviceContext *pContext;
IDXGISwapChain* pSwapChain;

void resize(int w, int h)
{
}

void keyboard(unsigned char c, int x, int y)
{
}

void s_keyboard(int b, int x, int y)
{
}

void mouse(int b, int p, int x, int y)
{
}

void motion(int b, int x, int y)
{
}

void p_motion(int x, int y)
{
}

void render()
{

}


bool chooseAdapter(IDXGIAdapter*& oAdapter, DXGI_ADAPTER_DESC& adapDesc)
{
    // Reset the value
    oAdapter = NULL;

    // Create a DXGIFactory object.
    IDXGIFactory* pFactory = NULL;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory)))
    {
        return false;
    }

    // Enumuerate the adapters
    IDXGIAdapter *pAdapter = NULL;
    SIZE_T        lastVideoMemSize = 0;
    for (unsigned int i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        DXGI_ADAPTER_DESC aDesc;
        if (pAdapter && (S_OK == pAdapter->GetDesc(&aDesc))){
            if (aDesc.DedicatedVideoMemory > lastVideoMemSize){
                lastVideoMemSize = aDesc.DedicatedVideoMemory;
                oAdapter = pAdapter;
                // Save desc
                adapDesc.VendorId = aDesc.VendorId;
                adapDesc.DeviceId = aDesc.DeviceId;
                adapDesc.SubSysId = aDesc.SubSysId;
                adapDesc.Revision = aDesc.Revision;
                adapDesc.DedicatedVideoMemory = aDesc.DedicatedVideoMemory;
                adapDesc.DedicatedSystemMemory = aDesc.DedicatedSystemMemory;
                adapDesc.SharedSystemMemory = aDesc.SharedSystemMemory;
                adapDesc.AdapterLuid = aDesc.AdapterLuid;
                wcscpy_s(adapDesc.Description, sizeof(aDesc.Description) / sizeof(WCHAR), aDesc.Description);
            }
        }
    }

    // Release factory
    if (pFactory)
    {
        pFactory->Release();
        pFactory = NULL;
    }

    return true;
}


bool createDevice(ZH::Widgets::WindowsInfo* winInfo)
{
    HRESULT hr = S_OK;

    // Check win info
    if (winInfo){
        if ((winInfo->m_width <= 0) ||
            (winInfo->m_height <= 0) ||
            (!winInfo->m_winHandle)){
            return false;
        }
    }

    // Device creating flag
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    //            createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // Driver types
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE/*,
                                D3D_DRIVER_TYPE_WARP,
                                D3D_DRIVER_TYPE_REFERENCE*/
    };
    const UINT numDriverTypes = ARRAYSIZE(driverTypes);

    // Feature levels
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    const UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    // Choose adapter
    IDXGIAdapter* pAdapter = NULL;
    DXGI_ADAPTER_DESC m_sAdapterDesc;
    chooseAdapter(pAdapter, m_sAdapterDesc);

    // Create device only
    if (!winInfo){
        // Create device
        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
        {
            hr = D3D11CreateDevice(
                NULL,                                // NULL to use first video adapter
                driverTypes[driverTypeIndex],        // Hardware -> Warp -> Software
                NULL,                                // NULL for software implementation
                createDeviceFlags,                   // Device creating flags
                featureLevels,                       // Feature levels
                numFeatureLevels,                    // Number of feature levels
                D3D11_SDK_VERSION,                   // SDK Version
                &pDevice,                          // Device
                &featureLevel,                     // Feature level
                &pContext                          // Context
                );
            if (SUCCEEDED(hr))
                break;
        }
    }
    // Create device with swap chain
    else{
        // Swap chain description
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 1;
        sd.BufferDesc.Width = winInfo->m_width;
        sd.BufferDesc.Height = winInfo->m_height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = winInfo->m_winHandle;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        // Create device and swap chain
        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
        {
            hr = D3D11CreateDeviceAndSwapChain(
                NULL,                                // NULL to use first video adapter
                driverTypes[driverTypeIndex],        // Hardware -> Warp -> Software
                NULL,                                // NULL for software implementation
                createDeviceFlags,                   // Device creating flags
                featureLevels,                       // Feature levels
                numFeatureLevels,                    // Number of feature levels
                D3D11_SDK_VERSION,                   // SDK Version
                &sd,                                 // Swap chain description
                &pSwapChain,                       // Swap chain
                &pDevice,                          // Device
                &featureLevel,                     // Feature level
                &pContext                          // Context
                );
            if (SUCCEEDED(hr))
                break;
        }
    }

    if (FAILED(hr)){
        return false;
    }

    return true;
}

bool doTest()
{
    // BUFFER DESC
    D3D11_BUFFER_DESC d3d_buff_desc;
    d3d_buff_desc.ByteWidth = sizeof(float) * 120;
    d3d_buff_desc.Usage = D3D11_USAGE_DYNAMIC;
    d3d_buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    d3d_buff_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    d3d_buff_desc.MiscFlags = 0;
    // SUBRESOURCE_DATA
    D3D11_SUBRESOURCE_DATA d3d_subresource_data;
    d3d_subresource_data.pSysMem = new float[120];

    // Create buffer
    ID3D11Buffer* d3d_buffer = NULL;
    HRESULT hr = pDevice->CreateBuffer(&d3d_buff_desc, &d3d_subresource_data, &d3d_buffer);
    if ((S_OK == hr) && d3d_buffer){
        return true;
    }
    else{
        return false;
    }

    int clewStatus = clewInit("OpenCL.dll");


}


void main()
{
    pWin = ZH::Widgets::Window::Create(
        "jack", 100, 100, 800, 600, true);

    ZH::Widgets::WindowsInfo* info = pWin->GetWinInfo();
    bool reault = createDevice(info);

    doTest();


    pWin->RegisterWindowResizeFunc(resize);
    pWin->RegisterKeyboardFunc(keyboard);
    pWin->RegisterSpecialKeyboardFunc(s_keyboard);
    pWin->RegisterMousePressReleaseFunc(mouse);
    pWin->RegisterMouseMoveWithButtonPressedFunc(motion);
    pWin->RegisterMouseMoveFunc(p_motion);
    pWin->RegisterRenderFunc(render);

    pWin->ShowWindow();
    pWin->EnterMsgLoop();


    ZH::Widgets::Window::Destroy(pWin);
}