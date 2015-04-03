#ifndef TEXTURE_UTILS_H
#define TEXTURE_UTILS_H


#include <GL/glew.h>

bool GetTextureInfo2D(GLenum target, GLuint texture, GLint level,
    GLint* w, GLint* h, GLint* fmt);

bool ReadTexture2D(GLenum target, GLuint texture, GLuint level,
    GLint x, GLint y, GLint w, GLint h, void** data);

class PixelReadStateHelper
{
public:
    PixelReadStateHelper();
    ~PixelReadStateHelper(){}

    void TakeSnapshot();
    void RestoreSnapshot() const;

private:
    GLint m_readFrameBuffer;
    GLint m_readBuffer;
    GLint m_pixelPackBuffer;
    GLint m_packRowLength;
    GLint m_packImageHeight;
    GLint m_packSkipRows;
    GLint m_packSkipPixels;
    GLint m_packSkipImages;
    GLint m_packAlignment;
    //GL_PACK_SWAP_BYTES  // default false, non-GLES, how to deal ?
    //GL_PACK_LSB_FIRST  // default false, non-GLES, how to deal ?
};

class PixelWriteStateHelper
{
public:
    PixelWriteStateHelper();
    ~PixelWriteStateHelper(){}

    void TakeSnapshot();
    void RestoreSnapshot() const;

private:
    GLint m_pixelUnpackBuffer;
    GLint m_unpackRowLength;
    GLint m_unpackImageHeight;
    GLint m_unpackSkipRows;
    GLint m_unpackSkipPixels;
    GLint m_unpackSkipImages;
    GLint m_unpackAlignment;
    //GL_UNPACK_SWAP_BYTES  // default false, non-GLES, how to deal ?
    //GL_UNPACK_LSB_FIRST  // default false, non-GLES, how to deal ?
};

class ApicTextureSnapshot
{
public:
    ApicTextureSnapshot(void);
    ~ApicTextureSnapshot(void);

    bool TakeSnapshot(
        GLenum target,
        GLuint texture,
        GLint  level,
        GLint  x,
        GLint  y,
        GLint  w,
        GLint  h
        );

    bool TakeSnapshot(
        GLenum target,
        GLuint texture,
        GLint  level
        );

    bool RestoreSnapshot(void);


    /**
    * Reset to the initial state, no data cached inside.
    */
    void Reset(void);

    static bool IsTextureTargetSupported(GLenum target);

    static const GLenum m_sFormat = GL_RGBA;
    static const GLenum m_sType   = GL_UNSIGNED_BYTE;
private:
    GLenum m_textureTarget;
    GLuint m_texture;
    GLuint m_level;
    GLint  m_x;
    GLint  m_y;
    GLsizei m_w;
    GLsizei m_h;
    GLint m_internalFmt;

    /**
    * Buffer name of snapshotted target.
    */
    GLuint m_buffer;

    /**
    * Client space used to hold the snapshotted buffer data.
    */
    char*  m_pClientData;
};















#endif