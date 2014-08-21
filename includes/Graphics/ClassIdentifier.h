#ifndef CLASS_IDENTIFIER_H
#define CLASS_IDENTIFIER_H

namespace ZH{
    namespace Graphics{

        enum E_CLASS_ID{
            E_CID_DEVICE,
            E_CID_SHADER,
            E_CID_PROGRAM

        };

        #define CLASS_IDENTIFIER( C ) \
            public: \
            static const E_CLASS_ID m_eClassID = C;

    }
}


#endif
