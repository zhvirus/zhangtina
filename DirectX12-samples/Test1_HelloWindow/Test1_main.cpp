// DirectX12-samples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "Widget\Window.h"
#include "DX12Util.h"

const static unsigned int frameWidth = 800;
const static unsigned int frameHeight = 600;
HWND gWndHandle = nullptr;

ComPtr<IDXGISwapChain3> gSwapChain;
ComPtr<ID3D12Device> gDevice;
ComPtr<ID3D12CommandQueue> gCommandQueue;
ComPtr<ID3D12DescriptorHeap> gRtvHeap;
ComPtr<ID3D12CommandAllocator> gCommandAllocator;
UINT gRtvDescriptorSize;
ComPtr<ID3D12PipelineState> gPipelineState;
ComPtr<ID3D12GraphicsCommandList> gCommandList;

const unsigned int FrameCount = 2;
ComPtr<ID3D12Resource> m_renderTargets[FrameCount];

// fence
HANDLE gFenceEvent;
ComPtr<ID3D12Fence> gFence;
UINT64 gFenceValue;

unsigned int gFrameIndex = 0;



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

void PopulateCommandList()
{
    // Command list allocators can only be reset when the associated 
    // command lists have finished execution on the GPU; apps should use 
    // fences to determine GPU execution progress.
    DX12Util::ThrowIfFailed(gCommandAllocator->Reset());

    // However, when ExecuteCommandList() is called on a particular command 
    // list, that command list can then be reset at any time and must be before 
    // re-recording.
    DX12Util::ThrowIfFailed(gCommandList->Reset(gCommandAllocator.Get(), gPipelineState.Get()));

    // Indicate that the back buffer will be used as a render target.
    gCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[gFrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(gRtvHeap->GetCPUDescriptorHandleForHeapStart(), gFrameIndex, gRtvDescriptorSize);

    // Record commands.
    const float clearColor[] = { 1.0f, 0.2f, 0.4f, 1.0f };
    gCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // Indicate that the back buffer will now be used to present.
    gCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[gFrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    DX12Util::ThrowIfFailed(gCommandList->Close());
}

static void WaitForPreviousFrame()
{
    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
    // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
    // sample illustrates how to use fences for efficient resource usage and to
    // maximize GPU utilization.

    // Signal and increment the fence value.
    const UINT64 fence = gFenceValue;
    DX12Util::ThrowIfFailed(gCommandQueue->Signal(gFence.Get(), fence));
    gFenceValue++;

    // Wait until the previous frame is finished.
    if (gFence->GetCompletedValue() < fence)
    {
        DX12Util::ThrowIfFailed(gFence->SetEventOnCompletion(fence, gFenceEvent));
        WaitForSingleObject(gFenceEvent, INFINITE);
    }

    gFrameIndex = gSwapChain->GetCurrentBackBufferIndex();
}

static void OnRender()
{
    // Record all the commands we need to render the scene into the command list.
    PopulateCommandList();

    // Execute the command list.
    ID3D12CommandList* ppCommandLists[] = { gCommandList.Get() };
    gCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Present the frame.
    DX12Util::ThrowIfFailed(gSwapChain->Present(1, 0));

    WaitForPreviousFrame();
}

void render()
{
    //OnUpdate();
    OnRender();
}

static void LoadPipeline()
{
#if defined(_DEBUG)
    // Enable the D3D12 debug layer.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
        }
    }
#endif

    ComPtr<IDXGIFactory4> factory;
    DX12Util::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

    // Create device
    {
        if (/*m_useWarpDevice*/false)
        {
            ComPtr<IDXGIAdapter> warpAdapter;
            DX12Util::ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

            DX12Util::ThrowIfFailed(D3D12CreateDevice(
                warpAdapter.Get(),
                D3D_FEATURE_LEVEL_12_0,
                IID_PPV_ARGS(&gDevice)
                ));
        }
        else
        {
            ComPtr<IDXGIAdapter1> hardwareAdapter;
            DX12Util::GetHardwareAdapter(factory.Get(), &hardwareAdapter);

            DX12Util::ThrowIfFailed(D3D12CreateDevice(
                hardwareAdapter.Get(),
                D3D_FEATURE_LEVEL_12_0,
                IID_PPV_ARGS(&gDevice)
                ));
        }
    }

    // Create command queue
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        DX12Util::ThrowIfFailed(gDevice->CreateCommandQueue(&queueDesc,
            IID_PPV_ARGS(&gCommandQueue)));
    }

    // Create Swap Chain
    {
        ComPtr<IDXGISwapChain1> swapChain;

        // Describe and create the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = FrameCount;
        swapChainDesc.Width = frameWidth;
        swapChainDesc.Height = frameHeight;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count = 1;

        DX12Util::ThrowIfFailed(factory->CreateSwapChainForHwnd(
            gCommandQueue.Get(),// Swap chain needs the queue so that it can force a flush on it.
            gWndHandle,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain
            ));

        DX12Util::ThrowIfFailed(swapChain.As(&gSwapChain));

        // This sample does not support fullscreen transitions.
        DX12Util::ThrowIfFailed(factory->MakeWindowAssociation(
            gWndHandle, DXGI_MWA_NO_ALT_ENTER));

        gFrameIndex = gSwapChain->GetCurrentBackBufferIndex();
    }

    // Create descriptor heaps.
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = FrameCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        DX12Util::ThrowIfFailed(gDevice->CreateDescriptorHeap(
            &rtvHeapDesc, IID_PPV_ARGS(&gRtvHeap)));

        gRtvDescriptorSize = gDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    // Create frame resources.
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
            gRtvHeap->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV for each frame.
        for (UINT n = 0; n < FrameCount; n++)
        {
            DX12Util::ThrowIfFailed(gSwapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
            gDevice->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, gRtvDescriptorSize);
        }
    }

    // Create command allocator
    DX12Util::ThrowIfFailed(gDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&gCommandAllocator)));

}

static void LoadAssets()
{
    // Create the command list.
    {
        DX12Util::ThrowIfFailed(gDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, gCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&gCommandList)));

        // Command lists are created in the recording state, but there is nothing
        // to record yet. The main loop expects it to be closed, so close it now.
        DX12Util::ThrowIfFailed(gCommandList->Close());
    }

    // Create synchronization objects.
    {
        DX12Util::ThrowIfFailed(gDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&gFence)));
        gFenceValue = 1;

        // Create an event handle to use for frame synchronization.
        gFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (gFenceEvent == nullptr)
        {
            DX12Util::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }
    }

}

void OnInit()
{
    LoadPipeline();
    LoadAssets();
}

int main()
{
    std::shared_ptr<ZH::Widgets::Window> pWin =
        ZH::Widgets::WindowFactory::instance().createWindow(
        "jack", 100, 100, frameWidth, frameHeight, true);

    gWndHandle = pWin->GetWinInfo()->m_winHandle;

    OnInit();

    pWin->RegisterWindowResizeFunc(resize);
    pWin->RegisterKeyboardFunc(keyboard);
    pWin->RegisterSpecialKeyboardFunc(s_keyboard);
    pWin->RegisterMousePressReleaseFunc(mouse);
    pWin->RegisterMouseMoveWithButtonPressedFunc(motion);
    pWin->RegisterMouseMoveFunc(p_motion);
    pWin->RegisterRenderFunc(render);

    pWin->ShowWindow();
    pWin->EnterMsgLoop();


    ZH::Widgets::WindowFactory::instance().destroyWindow(pWin);

    return 0;
}

