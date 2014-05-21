#pragma once

// Forwards
namespace ZH{
    namespace Graphics{
        class Device;
        class RenderFragment;
    }

    namespace Widgets{
        class WindowsInfo;
    }
}

class Global
{
public:
    static Global& instance(){ static Global g; return g; }
    static void print( const char* const, ...);
    static void error( const char* const, ...);

public:
    bool startDevice( ZH::Widgets::WindowsInfo& );
    ZH::Graphics::RenderFragment* defaultRenderFragment(){ return m_pRenderFragment; };
    ZH::Graphics::RenderFragment* createDefaultRenderFragment( ZH::Widgets::WindowsInfo& );
private:


private:
    ZH::Graphics::Device* m_pDevice;
    ZH::Graphics::RenderFragment* m_pRenderFragment;


private:
    Global();
    virtual ~Global();
    bool operator = ( const Global& ){}
    Global( const Global& ){}
};

