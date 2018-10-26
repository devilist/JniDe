//
// Created by zengp on 2018/10/21.
//

#include "include/preview_render.h"
#include "../libutils/my_log.h"

#define LOG_TAG "PreviewRender"

enum
{
    ATTRIBUTE_VERTEX, ATTRIBUTE_TEXCOORD,
};

PreviewRender::PreviewRender()
{

}

PreviewRender::~PreviewRender()
{

}

bool PreviewRender::init(int width, int height, PreviewTexture *texture)
{
    MY_LOGE(LOG_TAG, "PreviewRender init");
    this->backingLeft = 0;
    this->backingTop = 0;
    this->backingWidth = width;
    this->backingHeight = height;
    this->texture = texture;

    vertShader = 0;
    fragShader = 0;
    program = 0;

    int res = initShaders();
    if (res < 0) {
        MY_LOGI(LOG_TAG, "PreviewRender init shader failed ");
        return false;
    }

    res = this->useProgram();
    if (res < 0) {
        MY_LOGI(LOG_TAG, "PreviewRender use program failed ");
        this->dealloc();
        return false;
    }
    return false;
}

void PreviewRender::render()
{
    MY_LOGE(LOG_TAG, "PreviewRender render");
    glViewport(backingLeft, backingTop, backingWidth, backingHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(program);
    // 设置物体顶点坐标
    glVertexAttribPointer(ATTRIBUTE_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(ATTRIBUTE_VERTEX);
    // 设置纹理坐标
    glVertexAttribPointer(ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
    glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);
    // 激活纹理，将要绘制的纹理传给shader
    texture->bindTexture(uniformSampler);
    // 绘制
    glDrawArrays(GL_TRIANGLE_STRIP,
                 0,    // 顶点数组中的起始顶点索引
                 4);   // 绘制的顶点数量
}

void PreviewRender::dealloc()
{
    if (vertShader) {
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDeleteShader(fragShader);
    }
    if (texture) {
        texture->dealloc();
    }
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
}

void PreviewRender::resetRenderSize(int left, int top, int width, int height)
{
    this->backingLeft = left;
    this->backingTop = top;
    this->backingWidth = width;
    this->backingHeight = height;
}

int PreviewRender::useProgram()
{
    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, vertShader);
    glBindAttribLocation(program, ATTRIBUTE_VERTEX, "position");
    glBindAttribLocation(program, ATTRIBUTE_TEXCOORD, "texcoord");
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        MY_LOGI(LOG_TAG, "Failed to link program %d", program);
        return -1;
    }
    glUseProgram(program);
    uniformSampler = glGetUniformLocation(program, "yuvTexSampler");

    return 1;
}

int PreviewRender::initShaders()
{
    vertShader = compileShader(GL_VERTEX_SHADER, PREVIEW_VERTEX_SHADER);
    if (!vertShader) {
        return -1;
    }
    fragShader = compileShader(GL_FRAGMENT_SHADER, PREVIEW_FRAG_SHADER);
    if (!fragShader) {
        return -1;
    }
    return 1;
}

GLuint PreviewRender::compileShader(GLenum type, const char *shaderSource)
{
    GLint status;
    GLuint shader = glCreateShader(type);
    if (shader == 0 || shader == GL_INVALID_ENUM) {
        MY_LOGI(LOG_TAG, "Failed to create shader %d", type);
        return 0;
    }
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glDeleteShader(shader);
        MY_LOGI(LOG_TAG, "Failed to compile shader : %s\n", shaderSource);
        return 0;
    }
    return shader;
}

bool PreviewRender::checkGlError(const char *where)
{
    GLint error;
    for (error = glGetError(); error; error = glGetError()) {
        MY_LOGE(LOG_TAG, "error: after %s() glError (0x%x)\n", where, error);
        return true;
    }
    return false;
}

