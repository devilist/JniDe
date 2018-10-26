//
// Created by zengp on 2018/10/21.
//


#ifndef JNIDE_PREVIEW_RENDER_H
#define JNIDE_PREVIEW_RENDER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "preview_texture.h"

static const char *PREVIEW_VERTEX_SHADER =
        "attribute vec4 position;       \n"
                "attribute vec2 texcoord;       \n"
                "varying vec2 v_texcoord;       \n"
                "void main(void)                \n"
                "{                              \n"
                "   gl_Position = position;     \n"
                "   v_texcoord = texcoord;      \n"
                "}                              \n";

static const char *PREVIEW_FRAG_SHADER =
        "varying highp vec2 v_texcoord;                         \n"
                "uniform sampler2D yuvTexSampler;                       \n"
                "void main() {                                          \n"
                "  gl_FragColor = texture2D(yuvTexSampler, v_texcoord); \n"
                "}\n";

//物体顶点坐标
static const GLfloat vertices[] = {-1.0f, 1.0f,
                                   -1.0f, -1.0f,
                                   1.0f, 1.0f,
                                   1.0f, -1.0f};

// 纹理坐标
static const GLfloat texCoords[] = {0.0f, 0.0f,
                                    0.0f, 1.0f,
                                    1.0f, 0.0f,
                                    1.0f, 1.0f};

class PreviewRender
{
protected:
    GLint backingLeft;
    GLint backingTop;
    GLint backingWidth;
    GLint backingHeight;

    GLuint vertShader;
    GLuint fragShader;

    PreviewTexture *texture;
    GLint uniformSampler;
    GLuint program;

public:
    PreviewRender();

    virtual ~PreviewRender();

    virtual bool init(int width, int height, PreviewTexture *texture);

    virtual void render();

    virtual void dealloc();

    void resetRenderSize(int left, int top, int width, int height);

protected:
    int useProgram();

    int initShaders();

    GLuint compileShader(GLenum type, const char *shaderSource);

    bool checkGlError(const char *where);
};

#endif //JNIDE_PREVIEW_RENDER_H
