#pragma once
#include "TinaX_Preference.h"

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

public:
    TinaX_Preference& pref() { return m_preference; }

    bool startDevice( ZH::Widgets::WindowsInfo& );
    ZH::Graphics::RenderFragment* defaultRenderFragment(){ return m_pRenderFragment; };
    ZH::Graphics::RenderFragment* createDefaultRenderFragment( ZH::Widgets::WindowsInfo& );

    void applyPrefToRender();

private:


private:
    TinaX_Preference m_preference;
    ZH::Graphics::Device* m_pDevice;
    ZH::Graphics::RenderFragment* m_pRenderFragment;


private:
    Global();
    virtual ~Global();
    bool operator = ( const Global& ){}
    Global( const Global& ){}
};

