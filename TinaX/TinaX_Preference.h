#pragma once
#include "Math/float4.h"

class TinaX_Preference
{
public:
    static TinaX_Preference& instance() { static TinaX_Preference p; return p;}

    void applyToRenderer();

    const ZH::Math::float4& clearColor()const { return m_clearColor; }
    void clearColor( const ZH::Math::float4& col );

private:
    void recover();

private:
    TinaX_Preference();
    ~TinaX_Preference();
    TinaX_Preference( const TinaX_Preference& ){}
    TinaX_Preference& operator = ( const TinaX_Preference& ) { return *this; }
private:
    ZH::Math::float4 m_clearColor;
};

