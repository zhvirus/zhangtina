#ifndef CLASS_IDENTIFIER_H
#define CLASS_IDENTIFIER_H

namespace ZH{
    namespace Graphics{

        enum E_CLASS_ID{
            E_CID_TEXTURE2D,
            E_CID_RENDER_ITEM,
            E_CID_RENDER_NODE,
            E_CID_RENDER_NODE_GRID,
            E_CID_CAMERA,
            E_CID_CAMERA_ORTHO,
            E_CID_CAMERA_PERSP,
            E_CID_DEVICE,
            E_CID_DEVICE_DX11,
            E_CID_DEVICE_GL,
            E_CID_EFFECT,
            E_CID_EFFECT_SOLID,
            E_CID_EFFECT_INSTANCE,
            E_CID_EFFECT_INSTANCE_SOLID,
            E_CID_GEOMETRY_INSTANCE,
            E_CID_INDEX_BUFFER,
            E_CID_VERTEX_BUFFER,
            E_CID_RENDER_FRAGMENT,
            E_CID_RENDER_TARGET,
            E_CID_WORLD,
            E_CID_SIMPLE_WORLD,





        };

        #define CLASS_IDENTIFIER( C ) \
            public: \
            static const E_CLASS_ID m_eClassID = C;

    }
}


#endif
