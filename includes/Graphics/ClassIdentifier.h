#ifndef CLASS_IDENTIFIER_H
#define CLASS_IDENTIFIER_H

namespace ZH{
    namespace Graphics{

        enum E_CLASS_ID{
            E_CID_DEVICE,
            E_CID_SHADER,
            E_CID_PROGRAM,
            E_CID_INDEXBUFFER,
            E_CID_VERTEXBUFFER

        };

        #define CLASS_IDENTIFIER( C ) \
            public: \
            static const E_CLASS_ID m_eClassID = C;

    }
}


#endif
