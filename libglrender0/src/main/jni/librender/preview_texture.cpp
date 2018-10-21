//
// Created by zengp on 2018/10/21.
//

#include "include/preview_texture.h"
#include "../libutils/my_log.h"

#define LOG_TAG "PreviewTexture"

PreviewTexture::PreviewTexture()
{

}

PreviewTexture::~PreviewTexture()
{

}

int PreviewTexture::initTexture()
{
    MY_LOGE(LOG_TAG, "PreviewTexture initTexture");
    // 创建1个纹理
    glGenTextures(1, &texture);
    // 绑定该纹理对象
    glBindTexture(GL_TEXTURE_2D, texture);
    // 操作纹理,设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 双线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // 设置纹理坐标的映射规则
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return 1;
}

bool PreviewTexture::checkGlError(const char *where)
{
    GLint error;
    for (error = glGetError(); error; error = glGetError()) {
        MY_LOGE(LOG_TAG, "error: after %s() glError (0x%x)\n", where, error);
        return true;
    }
    return false;
}

bool PreviewTexture::createTexture()
{
    MY_LOGE(LOG_TAG, "PreviewTexture createTexture");
    texture = 0;
    int ret = initTexture();
    if (ret < 0) {
        MY_LOGE(LOG_TAG, "PreviewTexture createTexture failed ");
        this->dealloc();
        return false;
    }
    return true;
}

void PreviewTexture::updateTexImage(GLubyte *pixels, int width, int height)
{
    MY_LOGE(LOG_TAG, "PreviewTexture updateTexImage");
    if (pixels) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        if (checkGlError("glBindTexture")) {
            return;
        }
        // load
        glTexImage2D(GL_TEXTURE_2D,
                     0,                    // 加载的mip级别
                     GL_RGBA,              // 纹理存储的内部格式
                     width, height,
                     0,                    // 兼容参数，0
                     GL_RGBA,              // 输入纹理数据格式
                     GL_UNSIGNED_BYTE,     // 输入像素数据类型
                     pixels                // 包含图像的实际像素数据
        );
    }
}

bool PreviewTexture::bindTexture(GLint uniformSampler)
{
    MY_LOGE(LOG_TAG, "PreviewTexture bindTexture");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (checkGlError("glBindTexture")) {
        return false;
    }
    // 将要绘制的纹理对象传给fragment shader
    glUniform1i(uniformSampler, 0);
    return true;
}

void PreviewTexture::dealloc()
{
    MY_LOGE(LOG_TAG, "PreviewTexture dealloc");
    if (texture) {
        glDeleteTextures(1, &texture);
    }
}
