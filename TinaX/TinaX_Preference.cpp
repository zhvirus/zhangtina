#include "stdafx.h"
#include "TinaX_Preference.h"
#include "TinaX.h"
#include "Global.h"
#include "Util/Print.h"

extern CTinaXApp theApp;

#define clear_color_key (_T("TinaX_CLEAR_COLOR"))

TinaX_Preference::TinaX_Preference(void):
    m_clearColor( 0.0f, 0.0f, 0.0f, 1.0f )
{
    recover();
}


TinaX_Preference::~TinaX_Preference(void)
{
}

void TinaX_Preference::recover()
{
    // Recover clear color
    LPBYTE pData;
    UINT size;
    theApp.GetBinary( clear_color_key, &pData, &size);
    memcpy( (void*)(&m_clearColor.val), (void*)pData, size );


}

void TinaX_Preference::clearColor( const ZH::Math::float4& col )
{
    m_clearColor = col;
    theApp.WriteBinary( clear_color_key, (LPBYTE)(&m_clearColor.val), sizeof(float)*4);
    ZH::Util::INF("ClearColor set to (%.2f, %.2f, %.2f)\n", col.r, col.g, col.b);
}

