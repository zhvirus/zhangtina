#include "gl_common.h"

#include <GdiPlus.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace Gdiplus;

namespace TEST_COM
{
    void peek_gl_errors( int line )
    {
        GLenum err_code = glGetError();
        if (err_code == 0){
            std::cout << "(Line: " <<std::dec<< line << ") No error" << std::endl;
            return;
        }
        
        while (err_code != 0){
            char* err_msg = (char*)glewGetErrorString(err_code);
            std::cout << "(Line: "<<line<<") GL ERROR :" << err_msg <<"( 0x"<<std::hex<<err_code<<")"<<std::endl;
            err_code = glGetError();
        }
    }


    // image
    image::image()
    {
        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    }

    image::~image()
    {
        GdiplusShutdown(gdiplusToken);
    }

    void image::read_image(const std::wstring& filename, void*& pData,
        unsigned int& w, unsigned int& h, unsigned int& elemSizeInBytes )
    {
        //Image 
        Bitmap img(filename.c_str());
        BitmapData data;

        w = img.GetWidth();
        h = img.GetHeight();
        PixelFormat pf = img.GetPixelFormat();
        
        Rect rct(0, 0, w, h);
        img.LockBits(&rct, ImageLockModeRead, pf, &data);
        char* pSrcData = (char*)data.Scan0;
        elemSizeInBytes = (unsigned int)(data.Stride / w);

        pData = new char[w*h*elemSizeInBytes];

        unsigned int destStride = w*elemSizeInBytes;
        unsigned int srcStride  = data.Stride;
        for (unsigned int ih = 0; ih < h; ++ih){
            memcpy(
                (void*)((char*)pData + destStride*ih),
                (void*)((char*)pSrcData + srcStride*ih),
                destStride);
        }

        img.UnlockBits(&data);
    }


}