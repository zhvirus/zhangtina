#pragma once
#include "Math/float4.h"

class TinaX_Preference
{
public:
    TinaX_Preference(void);
    ~TinaX_Preference(void);

    const ZH::Math::float4& clearColor()const { return m_clearColor; }
    void clearColor( const ZH::Math::float4& col );

private:
    void recover();

private:
    ZH::Math::float4 m_clearColor;
};

