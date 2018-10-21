//
// Created by zengp on 2018/10/21.
//

#ifndef JNIDE_PREVIEW_TEXTURE_H
#define JNIDE_PREVIEW_TEXTURE_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


class PreviewTexture
{
private:
    GLuint texture;

    int initTexture();

    bool checkGlError(const char *where);

public:
    PreviewTexture();

    virtual ~PreviewTexture();

    bool createTexture();

    void updateTexImage(GLubyte *pixels, int width, int height);

    bool bindTexture(GLint uniformSampler);

    void dealloc();
};


#endif //JNIDE_PREVIEW_TEXTURE_H
