#include "textureUtils.h"
//#include "commonUtils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/**
* Helper function to peek the GL errors
*/
static bool PeekGLErrors(bool printMsg = false)
{
    bool hasError = false;
    GLenum errorCode = glGetError();
    while (GL_NO_ERROR != errorCode)
    {
        hasError = true;
        if (printMsg)
        {
            printf("GL Error detected: 0x%x !\n", errorCode);
        }
        errorCode = glGetError();
    }
    return hasError;
}

// PixelReadStateHelper
PixelReadStateHelper::PixelReadStateHelper():
    m_readFrameBuffer(0),
    m_readBuffer(0),
    m_pixelPackBuffer(0),
    m_packRowLength(0),
    m_packImageHeight(0),
    m_packSkipRows(0),
    m_packSkipPixels(0),
    m_packSkipImages(0),
    m_packAlignment(4)
{
}

void PixelReadStateHelper::TakeSnapshot()
{
    // Save current read fb and buffer status
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &m_readFrameBuffer);
    glGetIntegerv(GL_READ_BUFFER, &m_readBuffer);

    // Save current pixel pack buffer
    glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &m_pixelPackBuffer);

    // Save pack status
    glGetIntegerv(GL_PACK_ROW_LENGTH,   &m_packRowLength);
    glGetIntegerv(GL_PACK_IMAGE_HEIGHT, &m_packImageHeight);
    glGetIntegerv(GL_PACK_SKIP_ROWS,    &m_packSkipRows);
    glGetIntegerv(GL_PACK_SKIP_PIXELS,  &m_packSkipPixels);
    glGetIntegerv(GL_PACK_SKIP_IMAGES,  &m_packSkipImages);
    glGetIntegerv(GL_PACK_ALIGNMENT,    &m_packAlignment);
    //GL_PACK_SWAP_BYTES  // false, non-GLES, how to deal ?
    //GL_PACK_LSB_FIRST  // false, non-GLES, how to deal ?
}

void PixelReadStateHelper::RestoreSnapshot() const
{
    // Restore previous read fb and buffer status
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_readFrameBuffer);
    glReadBuffer(m_readBuffer);

    // Restore previous pixel pack buffer
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pixelPackBuffer);

    // Restore pack status
    glPixelStorei(GL_PACK_ROW_LENGTH,   m_packRowLength);
    glPixelStorei(GL_PACK_IMAGE_HEIGHT, m_packImageHeight);
    glPixelStorei(GL_PACK_SKIP_ROWS,    m_packSkipRows);
    glPixelStorei(GL_PACK_SKIP_PIXELS,  m_packSkipPixels);
    glPixelStorei(GL_PACK_SKIP_IMAGES,  m_packSkipImages);
    glPixelStorei(GL_PACK_ALIGNMENT,    m_packAlignment);
    //GL_PACK_SWAP_BYTES  // false, non-GLES, how to deal ?
    //GL_PACK_LSB_FIRST  // false, non-GLES, how to deal ?
}


// PixelWriteStateHelper
PixelWriteStateHelper::PixelWriteStateHelper() :
    m_pixelUnpackBuffer(0),
    m_unpackRowLength(0),
    m_unpackImageHeight(0),
    m_unpackSkipRows(0),
    m_unpackSkipPixels(0),
    m_unpackSkipImages(0),
    m_unpackAlignment(4)
{
}

void PixelWriteStateHelper::TakeSnapshot()
{
    // Save current pixel unpack buffer
    glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &m_pixelUnpackBuffer);

    // Save unpack status
    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &m_unpackRowLength);
    glGetIntegerv(GL_UNPACK_IMAGE_HEIGHT, &m_unpackImageHeight);
    glGetIntegerv(GL_UNPACK_SKIP_ROWS, &m_unpackSkipRows);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &m_unpackSkipPixels);
    glGetIntegerv(GL_UNPACK_SKIP_IMAGES, &m_unpackSkipImages);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &m_unpackAlignment);
    //GL_UNPACK_SWAP_BYTES  // false, non-GLES, how to deal ?
    //GL_UNPACK_LSB_FIRST  // false, non-GLES, how to deal ?
}

void PixelWriteStateHelper::RestoreSnapshot() const
{
    // Restore previous pixel pack buffer
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pixelUnpackBuffer);

    // Restore pack status
    glPixelStorei(GL_UNPACK_ROW_LENGTH, m_unpackRowLength);
    glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, m_unpackImageHeight);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, m_unpackSkipRows);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, m_unpackSkipPixels);
    glPixelStorei(GL_UNPACK_SKIP_IMAGES, m_unpackSkipImages);
    glPixelStorei(GL_UNPACK_ALIGNMENT, m_unpackAlignment);
    //GL_UNPACK_SWAP_BYTES  // false, non-GLES, how to deal ?
    //GL_UNPACK_LSB_FIRST  // false, non-GLES, how to deal ?
}

bool GetTextureSize2D(GLenum target, GLuint texture, GLint level, GLint* w, GLint* h)
{
    // Sanity check
    if (!w || !h || !glIsTexture(texture))
    {
        return false;
    }

    if (!ApicTextureSnapshot::IsTextureTargetSupported(target))
    {
        printf("ERROR: GetTextureSize2D(target=0x%x) - unsupported target!\n",
            target);
        return false;
    }

    if (PeekGLErrors())
    {
        printf("WARNING - GL error detected before "
            "GetTextureSize2D(texture=%u)\n",
            texture);
    }

    // Bind target should use
    const GLenum texTargetQuery = (target == GL_TEXTURE_2D) ? GL_TEXTURE_BINDING_2D : GL_TEXTURE_BINDING_CUBE_MAP;
    const GLenum texTargetBinding = (target == GL_TEXTURE_2D) ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;

    GLint oldTexBinding = 0;
    glGetIntegerv(texTargetQuery, &oldTexBinding);
    glBindTexture(texTargetBinding, texture);

    glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, w);
    glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, h);

    glBindTexture(texTargetBinding, oldTexBinding);

    if (PeekGLErrors(true))
    {
        printf("WARNING - GL error detected after "
            "GetTextureSize2D(texture=%u)\n",
            texture);
    }

    if (*w > 0 && *h > 0)
    {
        return true;
    }

    return false;
}

bool ReadTexture2D(GLenum target, GLuint texture, GLint level,
    GLint x, GLint y, GLint w, GLint h, void** data)
{
    // Must pass in a valid and big enough client memory
    if (!data)
    {
        return false;
    }

    // Peek GL errors at the beginning
    if (PeekGLErrors())
    {
        printf("WARNING - GL error detected before "
            "ReadTexture2D(texture=%u)\n",
            texture);
    }

    // Check target
    if (!ApicTextureSnapshot::IsTextureTargetSupported(target))
    {
        printf("ERROR: ReadTexture2D(texture=%u) - texure target 0x%x not supported!\n",
            texture, target);
        return false;
    }

    // Check texture name
    if (!glIsTexture(texture))
    {
        printf("ERROR: ReadTexture2D(texture=%u) - texure is not valid!\n",
            texture);
        return false;
    }

    // Check read geometry
    if (x < 0 || y < 0 || w <= 0 || h <= 0)
    {
        printf("ERROR: ReadTexture2D(texture=%u) - read region is invalid!"
            " (x=%d, y=%d, w=%d, h=%d)\n", texture, x, y, w, h);
        return false;
    }

    GLint texWidth = 0;
    GLint texHeight = 0;
    if (!GetTextureSize2D(target, texture, level, &texWidth, &texHeight))
    {
        printf("ERROR: ReadTexture2D(texture=%u) - get texture size failed!\n",
            texture);
        return false;
    }

    if ((x + w > texWidth) || (y + h > texHeight))
    {
        printf("ERROR: ReadTexture2D(texture=%u) - Reading geometry(x=%d, y=%d, w=%d, h=%d)"
            " exceeds texture geometry(width=%d, height=%d)\n",
            texture, x, y, w, h, texWidth, texHeight);
        return false;
    }

    // Save pixel read related states
    PixelReadStateHelper readStateHelper;
    readStateHelper.TakeSnapshot();

    // Set GL_PIXEL_PACK_BUFFER 0
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    // Set pack status
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_IMAGE_HEIGHT, 0);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_PACK_SKIP_IMAGES, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    // Bind texture
    const GLenum texTargetQuery = (target == GL_TEXTURE_2D) ? GL_TEXTURE_BINDING_2D : GL_TEXTURE_BINDING_CUBE_MAP;
    const GLenum texTargetBinding = (target == GL_TEXTURE_2D) ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
    GLint oldTexBinding = 0;
    glGetIntegerv(texTargetQuery, &oldTexBinding);
    glBindTexture(texTargetBinding, texture);

    // Set texture to read status
    GLuint newReadFB = 0;
    glGenFramebuffers(1, &newReadFB);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, newReadFB);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        target, texture, level);
    glReadBuffer(GL_COLOR_ATTACHMENT0);


    // Read pixels
    // glReadPixels reads from the low-left, but this function
    // treats (x,y) as top-left.
    // Also the later restore function like glTexImageSubData2D()
    // also use top-left.
    *data = new char[4 * w*h];
    glReadPixels(x, y, w, h, ApicTextureSnapshot::m_sFormat,
        ApicTextureSnapshot::m_sType, *data);

    // Flip data in vertical
    //if (h >= 2)
    //{
    //    assert(ApicTextureSnapshot::m_sFormat == GL_RGBA);
    //    assert(ApicTextureSnapshot::m_sType == GL_UNSIGNED_BYTE);
    //    const int lineStride = w * 4;
    //    char* pTempLine = new char[lineStride];
    //    const int loopLines = h / 2;
    //    for (int line = 0; line < loopLines; ++line)
    //    {
    //        const int swapline = h - line - 1;
    //        void* ptr1 = (void*)((char*)(*data) + line*lineStride);
    //        void* ptr2 = (void*)((char*)(*data) + swapline*lineStride);
    //        memcpy(pTempLine, ptr1, lineStride);
    //        memcpy(ptr1, ptr2, lineStride);
    //        memcpy(ptr2, pTempLine, lineStride);
    //    }
    //    delete[]pTempLine;
    //}

    // Restore texture binding
    glBindTexture(texTargetBinding, oldTexBinding);

    // Restore pixel read related states
    readStateHelper.RestoreSnapshot();

    // Delete auxiliary frame buffer
    glDeleteFramebuffers(1, &newReadFB);

    if (PeekGLErrors(true))
    {
        printf("ERROR - GL error detected after "
            "ReadTexture2D(texture=%u)\n",
            texture);
        return false;
    }

    return true;
}


ApicTextureSnapshot::ApicTextureSnapshot(void) :
    m_pClientData(NULL)
{
    Reset();
}

ApicTextureSnapshot::~ApicTextureSnapshot(void)
{
    Reset();
}

void ApicTextureSnapshot::Reset(void)
{
    m_textureTarget = GL_TEXTURE_2D;
    m_texture = 0;
    m_level = 0;
    m_x = 0;
    m_y = 0;
    m_w = 0;
    m_h = 0;
    m_buffer = 0;
    
    if (m_pClientData)
    {
        delete[]m_pClientData;
        m_pClientData = NULL;
    }
}


bool ApicTextureSnapshot::IsTextureTargetSupported(GLenum target)
{
    return (target == GL_TEXTURE_2D ||
        target == GL_TEXTURE_CUBE_MAP_POSITIVE_X ||
        target == GL_TEXTURE_CUBE_MAP_NEGATIVE_X ||
        target == GL_TEXTURE_CUBE_MAP_POSITIVE_Y ||
        target == GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ||
        target == GL_TEXTURE_CUBE_MAP_POSITIVE_Z ||
        target == GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
}

bool ApicTextureSnapshot::TakeSnapshot(
    GLenum target,
    GLuint texture,
    GLint  level
    )
{
    GLint width = 0;
    GLint height = 0;
    if (!GetTextureSize2D(target, texture, level, &width, &height))
    {
        printf("ERROR - TakeSnapshot(texture=%u): Failed to get texture size!\n",
            texture);
        return false;
    }

    return TakeSnapshot(target, texture, level, 0, 0, width, height);
}

bool ApicTextureSnapshot::TakeSnapshot(
    GLenum target,
    GLuint texture,
    GLint  level,
    GLint  x,
    GLint  y,
    GLint  w,
    GLint  h
    )
{
    bool result = true;

    // Peek GL errors at the beginning
    if (PeekGLErrors())
    {
        printf("WARNING - GL error detected before "
            "TakeSnapshot(texture=%u)\n",
            texture);
    }

    // Reset
    Reset();

    // Read texture to client memory
    if (!ReadTexture2D(target, texture, level, x, y, w, h, (void**)&m_pClientData))
    {
        printf("ERROR - TakeSnapshot(texture=%u) ReadTexture2D failed!\n",
            texture);
        Reset();
        return false;  
    }

    // Check result
    if (PeekGLErrors(true))
    {
        printf("ERROR - GL error detected after "
            "TakeSnapshot(texture=%u)\n",
            texture);
        result = false;
    }
    else
    {
        // Everything is ok, save the snapshot information
        m_textureTarget = target;
        m_texture = texture;
        m_level = level;
        m_x = x;
        m_y = y;
        m_w = w;
        m_h = h;
        assert(m_pClientData);
    }

    if (!result)
    {
        Reset();
    }

    return result;
}

bool ApicTextureSnapshot::RestoreSnapshot(void)
{
    // Peek GL errors at the beginning
    if (PeekGLErrors())
    {
        printf("WARNING - GL error detected before "
            "RestoreSnapshot(texture=%u)\n",
            m_texture);
    }

    // Check saved texture
    if (GL_FALSE == glIsTexture(m_texture))
    {
        return false;
    }

    // Check geometry
    assert(m_w > 0);
    assert(m_h > 0);

    if (0 != m_buffer)
    {
        // Check saved buffer
        if (GL_FALSE == glIsBuffer(m_buffer))
        {
            printf("ERROR: RestoreSnapshot(texture=%u) - Invalid saved buffer %u !\n",
                m_texture, m_buffer);
            return false;
        }

        // Restore texture from saved buffer
    }
    else if (m_pClientData)
    {
        // Restore texture from saved client memory

        // Save pixel write related states
        PixelWriteStateHelper writeStateHelper;
        writeStateHelper.TakeSnapshot();

        // Set GL_PIXEL_PACK_BUFFER 0
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

        // Set pack status
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_SKIP_IMAGES, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Bind target should use
        const GLenum texTargetQuery = (m_textureTarget == GL_TEXTURE_2D) ? GL_TEXTURE_BINDING_2D : GL_TEXTURE_BINDING_CUBE_MAP;
        const GLenum texTargetBinding = (m_textureTarget == GL_TEXTURE_2D) ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;

        GLint oldTexBinding = 0;
        glGetIntegerv(texTargetQuery, &oldTexBinding);
        glBindTexture(texTargetBinding, m_texture);

        glTexSubImage2D(m_textureTarget, m_level, m_x, m_y,
            m_w, m_h, m_sFormat, m_sType, m_pClientData);

        glBindTexture(texTargetBinding, oldTexBinding);

        // Restore pixel write related states
        writeStateHelper.RestoreSnapshot();
    }
    else
    {
        printf("ERROR: RestoreSnapshot(texture=%u) - snapshotted texture not in good condition, failed!\n",
            m_texture);
        return false;
    }

    if (PeekGLErrors(true))
    {
        printf("ERROR - GL error detected after "
            "RestoreSnapshot(texture=%u)\n",
            m_texture);
        return false;
    }

    return true;
}