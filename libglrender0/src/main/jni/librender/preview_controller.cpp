//
// Created by admin on 2018/10/22.
//

#include "include/preview_controller.h"
#include "../libutils/my_log.h"
#include "../libpngdecoder/include/image.h"

#define LOG_TAG "PreviewController"

PreviewController::PreviewController()
{
    MY_LOGE(LOG_TAG, "PreviewController created");
    this->renderMsg = MSG_NONE;
    this->previewSurface = 0;
    this->eglCore = 0;
    this->screenWidth = 720;
    this->screenHeight = 720;
    this->pngDecoder = NULL;
    this->render = new PreviewRender();

    pthread_mutex_init(&pLock, NULL);
    pthread_cond_init(&pCondition, NULL);
}

PreviewController::~PreviewController()
{
    MY_LOGE(LOG_TAG, "PreviewController destroyed");
    pthread_mutex_destroy(&pLock);
    pthread_cond_destroy(&pCondition);
}

void PreviewController::setWindow(ANativeWindow *window)
{
    MY_LOGE(LOG_TAG, "PreviewController setWindow");
    pthread_mutex_lock(&pLock);
    renderMsg = MSG_WINDOW_SET;
    nativeWindow = window;
    pthread_cond_signal(&pCondition);
    pthread_mutex_unlock(&pLock);
}

void PreviewController::resetSize(int width, int height)
{
    MY_LOGE(LOG_TAG, "PreviewController resetSize width:%d; height:%d", width, height);
    pthread_mutex_lock(&pLock);
    screenWidth = width;
    screenHeight = height;
    render->resetRenderSize(0, 0, width, height);
    pthread_cond_signal(&pCondition);
    MY_LOGE(LOG_TAG, "PreviewController renderMsg:%d", renderMsg);
    pthread_mutex_unlock(&pLock);

}

bool PreviewController::start(char *filePath)
{
    MY_LOGE(LOG_TAG, "PreviewController start, image path :%s", filePath);
    pngDecoder = new PngPicDecoder();
    pngDecoder->openFile(filePath);
    pthread_create(&pthread, 0, threadStartCallback, this);
    return true;
}

void PreviewController::stop()
{
    MY_LOGE(LOG_TAG, "PreviewController stopping");
    pthread_mutex_lock(&pLock);
    renderMsg = MSG_RENDER_LOOP_EXIT;
    pthread_cond_signal(&pCondition);
    pthread_mutex_unlock(&pLock);
    MY_LOGE(LOG_TAG, "PreviewController join the render thread");
    pthread_join(pthread, 0);
    MY_LOGE(LOG_TAG, "PreviewController stopped");

}

void *PreviewController::threadStartCallback(void *myself)
{
    PreviewController *controller = (PreviewController *) myself;
    controller->renderLoop();  // 开始循环
    pthread_exit(0);
    return 0;
}

void PreviewController::renderLoop()
{
    bool renderEnabled = true;
    while (renderEnabled) {
        pthread_mutex_lock(&pLock);
        switch (renderMsg) {
            case MSG_WINDOW_SET:
                initialize();
                break;
            case MSG_RENDER_LOOP_EXIT:
                renderEnabled = false;
                destroy();
                break;
            default:
                break;
        }
        renderMsg = MSG_NONE;
        if (eglCore) {
            eglCore->makeCurrent(previewSurface);
            this->drawFrame();
            pthread_cond_wait(&pCondition, &pLock);
            usleep(100 * 1000); // 100毫秒
        }
        pthread_mutex_unlock(&pLock);
    }
}

bool PreviewController::initialize()
{
    MY_LOGE(LOG_TAG, "PreviewController initialize after window set");
    //egl
    eglCore = new EGLCore();
    eglCore->init();
    previewSurface = eglCore->createWindowSurface(nativeWindow);
    eglCore->makeCurrent(previewSurface);
    // texture
    texture = new PreviewTexture();
    bool res = texture->createTexture();
    if (!res) {
        destroy();
        return false;
    }

    this->updateTextImage();
    bool isRenderInit = render->init(screenWidth, screenHeight, texture);
    if (!isRenderInit) {
        return false;
    }
    return true;
}

void PreviewController::updateTextImage()
{
    if (pngDecoder) {
        const RawImageData rawImageData = pngDecoder->getRawImageData();
        MY_LOGE(LOG_TAG, "rawImageData Meta: width : %d height : %d size : %d colorFormat : %d",
                rawImageData.width, rawImageData.height, rawImageData.size,
                rawImageData.gl_color_format);
        MY_LOGE(LOG_TAG, "colorFormat is %d", GL_RGBA);
        texture->updateTexImage((byte *) rawImageData.data, rawImageData.width,
                                rawImageData.height);
        pngDecoder->releaseRawImageData(&rawImageData);
    }
}

void PreviewController::drawFrame()
{
    render->render();
    if (!eglCore->swapBuffers(previewSurface)) {
        MY_LOGE(LOG_TAG, "eglSwapBuffers() returned error %d", glGetError());
    }
}

void PreviewController::destroy()
{
    MY_LOGE(LOG_TAG, "PreviewController destroy, dealloc...");
    if (NULL != render) {
        render->dealloc();
        delete render;
        render = NULL;
    }
    if (NULL != eglCore) {
        eglCore->releaseSurface(previewSurface);
        eglCore->release();
        eglCore = NULL;
    }
}
