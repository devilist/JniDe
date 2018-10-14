//
// Created by zengp on 2018/9/22.
//

#include "./include/egl_core.h"
#include "./libutils/my_log.h"

#define LOG_TAG "EGLCore"

EGLCore::EGLCore()
{
    MY_LOGD(LOG_TAG, "EGLCore constructor");
    egl_presentation_time_android = 0;
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
}

EGLCore::~EGLCore()
{
    MY_LOGD(LOG_TAG, "EGLCore destructor");
}

bool EGLCore::init()
{
    return this->init(NULL);
}

bool EGLCore::init(EGLContext shareContext)
{
    MY_LOGD(LOG_TAG, "EGLCore init start , shareContext = %p", shareContext);

    // 1.寻找显示设备
    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
        MY_LOGE(LOG_TAG, "eglGetDisplay() return error : %d", eglGetError());
        return false;
    }
    // 2.初始化显示设备
    if (!eglInitialize(display, 0, 0)) {
        MY_LOGE(LOG_TAG, "eglInitialize() return error : %d", eglGetError());
        return false;
    }
    // 3.android平台配置选项
    EGLint numConfigs;
    const EGLint attribs[] = {
            EGL_BUFFER_SIZE, 32,
            EGL_ALPHA_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };
    if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
        MY_LOGE(LOG_TAG, "eglChooseConfig() return error : %d", eglGetError());
        release();
        return false;
    }
    // 4.创建opengl上下文
    EGLint contextAttributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    if (!(context = eglCreateContext(display, config, shareContext, contextAttributes))) {
        MY_LOGE(LOG_TAG, "eglCreateContext() return error : %d", eglGetError());
        release();
        return false;
    }

    egl_presentation_time_android = (EGL_PRESENTATION_TIME_ANDROID_PROC) eglGetProcAddress(
            "egl_presentation_time_android");
    if (!egl_presentation_time_android) {
        MY_LOGE(LOG_TAG, "egl_presentation_time_android is not available!");
    }

    MY_LOGD(LOG_TAG, "EGLCore init success !");
    return true;
}

void EGLCore::release()
{
    MY_LOGD(LOG_TAG, "EGLCore release");
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display, context);
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
}

EGLSurface EGLCore::createWindowSurface(ANativeWindow *_window)
{
    MY_LOGD(LOG_TAG, "EGLCore createWindowSurface");
    // create EGLSurface,使opengl的输出可以渲染到设备屏幕上
    EGLSurface surface = NULL;
    EGLint format;
    if (!(eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format))) {
        MY_LOGE(LOG_TAG, "eglGetConfigAttrib() return error : %d", eglGetError());
        release();
        return NULL;
    }
    ANativeWindow_setBuffersGeometry(_window, 0, 0, format);
    if (!(surface = eglCreateWindowSurface(display, config, _window, 0))) {
        MY_LOGE(LOG_TAG, "eglCreateWindowSurface() return error : %d", eglGetError());
    }
    return surface;
}

EGLSurface EGLCore::createOffScreenSurface(int width, int height)
{
    MY_LOGD(LOG_TAG, "EGLCore createOffScreenSurface");
    // 离线渲染
    EGLSurface surface = NULL;
    EGLint pBufferAttributes[] = {
            EGL_WIDTH, width,
            EGL_HEIGHT, height,
            EGL_NONE, EGL_NONE};
    if (!(surface = eglCreatePbufferSurface(display, config, pBufferAttributes))) {
        MY_LOGE(LOG_TAG, "eglCreatePbufferSurface() return error : %d", eglGetError());
    }
    return surface;
}

bool EGLCore::makeCurrent(EGLSurface eglSurface)
{
    // 为线程绑定surface和context
    MY_LOGD(LOG_TAG, "EGLCore makeCurrent");
    return (bool) eglMakeCurrent(display, eglSurface, eglSurface, context);
}

bool EGLCore::swapBuffers(EGLSurface eglSurface)
{
    MY_LOGD(LOG_TAG, "EGLCore swapBuffers");
    return (bool) eglSwapBuffers(display, eglSurface);
}

void EGLCore::releaseSurface(EGLSurface eglSurface)
{
    MY_LOGD(LOG_TAG, "EGLCore releaseSurface");
    eglDestroySurface(display, eglSurface);
    eglSurface = EGL_NO_SURFACE;
}

int EGLCore::querySurface(EGLSurface eglSurface, int what)
{
    MY_LOGD(LOG_TAG, "EGLCore querySurface");
    int value = -1;
    eglQuerySurface(display, eglSurface, what, &value);
    return value;
}

void EGLCore::setPresentationTime(EGLSurface eglSurface, khronos_stime_nanoseconds_t nanoseconds)
{
    // 设置时间戳
    egl_presentation_time_android(display, eglSurface, nanoseconds);
}

EGLContext EGLCore::getContext()
{
    return context;
}

EGLDisplay EGLCore::getDisplay()
{
    return display;
}

EGLConfig EGLCore::getConfig()
{
    return config;
}
