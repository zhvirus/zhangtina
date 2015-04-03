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

bool GetTextureInfo2D(GLenum target, GLuint texture, GLint level, GLint* w, GLint* h, GLint* fmt)
{
    // Sanity check
    if (!glIsTexture(texture))
    {
        return false;
    }

    if (!ApicTextureSnapshot::IsTextureTargetSupported(target))
    {
        printf("ERROR: GetTextureInfo2D(target=0x%x) - unsupported target!\n",
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

    if (w)
    {
        glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, w);
    }

    if (h)
    {
        glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, h);
    }

    if (fmt)
    {
        glGetTexLevelParameteriv(target, level, GL_TEXTURE_INTERNAL_FORMAT, fmt);
    }

    glBindTexture(texTargetBinding, oldTexBinding);

    if (PeekGLErrors(true))
    {
        printf("WARNING - GL error detected after "
            "GetTextureInfo2D(texture=%u)\n",
            texture);
    }

    return true;
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
    GLint texFmt = 0;
    if (!GetTextureInfo2D(target, texture, level, &texWidth, &texHeight, &texFmt))
    {
        printf("ERROR: ReadTexture2D(texture=%u) - get texture size&format failed!\n",
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

    GLint readFmt = GL_RGBA;
    GLint readType = GL_UNSIGNED_BYTE;

    if (texFmt == GL_DEPTH_COMPONENT)
    {
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            target, texture, level);
        glReadBuffer(GL_NONE);
        readFmt  = GL_DEPTH_COMPONENT;
        readType = GL_FLOAT;
    }
    else if (texFmt == GL_DEPTH_STENCIL)
    {
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            target, texture, level);
        glReadBuffer(GL_NONE);

        readFmt = GL_DEPTH_STENCIL;
        readType = GL_UNSIGNED_INT_24_8;
    }
    else
    {
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            target, texture, level);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
    }

    PeekGLErrors(true);

    GLenum ret = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
    if (ret != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Not complete!\n");
    }

    // Read pixels
    *data = new char[4 * w*h];
    glReadPixels(x, y, w, h, readFmt,
        readType, *data);

    PeekGLErrors(true);

    // Restore texture binding
    glBindTexture(texTargetBinding, oldTexBinding);

    PeekGLErrors(true);

    // Restore pixel read related states
    readStateHelper.RestoreSnapshot();

    PeekGLErrors(true);

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
    m_internalFmt(0),
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
    m_internalFmt = 0;
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
    GLint fmt = 0;
    if (!GetTextureInfo2D(target, texture, level, &width, &height, &fmt))
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

    // 
    GLint width = 0;
    GLint height = 0;
    GLint fmt = 0;
    if (!GetTextureInfo2D(target, texture, level, &width, &height, &fmt))
    {
        printf("ERROR - TakeSnapshot(texture=%u): Failed to get texture size!\n",
            texture);
        return false;
    }

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
        m_internalFmt = fmt;
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



        // fix me
        GLint readFmt = GL_RGBA;
        GLint readType = GL_UNSIGNED_BYTE;

        if (m_internalFmt == GL_DEPTH_COMPONENT)
        {
            readFmt = GL_DEPTH_COMPONENT;
            readType = GL_FLOAT;
        }
        else if (m_internalFmt == GL_DEPTH_STENCIL)
        {
            readFmt = GL_DEPTH_STENCIL;
            readType = GL_UNSIGNED_INT_24_8;
        }

        glTexSubImage2D(m_textureTarget, m_level, m_x, m_y,
            m_w, m_h, readFmt, readType, m_pClientData);

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