#pragma once

// Forwards
namespace ZH{
    namespace Graphics{
        class Device;
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

private:
    ZH::Graphics::Device* m_pDevice;


private:
    Global();
    virtual ~Global();
    bool operator = ( const Global& ){}
    Global( const Global& ){}
};

