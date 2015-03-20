#ifndef GL_COMMON_H
#define GL_COMMON_H

#include <windows.h>
#include <string>

namespace TEST_COM{

    void peek_gl_errors( int line );

    class image
    {
    public:
        static image& instance() { static image img; return img; }
        void read_image(const std::wstring& filename, void*& pData,
            unsigned int& w, unsigned int& h, unsigned int& elemSizeInBytes, bool low2top = true);

    private:
        ULONG_PTR gdiplusToken;

        image();
        ~image();
        image(const image&){}
        image& operator=(const image&) { return *this; }
    };






}

#endif
