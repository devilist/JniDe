//
// Created by zengp on 2018/9/22.
//

#ifndef JNIDE_EGL_CORE_H
#define JNIDE_EGL_CORE_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/native_window.h>
#include <KHR/khrplatform.h>  // https://stackoverflow.com/questions/49683210/what-is-the-khr-platform-header-doing

#include <pthread.h>

// EGLAPIENTRYP 实际上是关键字 __stdcall
// 被这个关键字修饰的函数，其参数都是从右向左通过堆栈传递的
// __stdcall EGL_PRESENTATION_TIME_ANDROID_PROC
typedef EGLBoolean (EGLAPIENTRYP EGL_PRESENTATION_TIME_ANDROID_PROC)(EGLDisplay display,
                                                                     EGLSurface surface,
                                                                     khronos_stime_nanoseconds_t time);

class EGLCore
{
private:
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGL_PRESENTATION_TIME_ANDROID_PROC egl_presentation_time_android;

public:
    EGLCore();

    virtual ~EGLCore();

    bool init();

    bool init(EGLContext shareContext);

    void release();

    EGLSurface createWindowSurface(ANativeWindow *_window);

    EGLSurface createOffScreenSurface(int width, int height);

    bool makeCurrent(EGLSurface eglSurface);

    bool swapBuffers(EGLSurface eglSurface);

    void releaseSurface(EGLSurface eglSurface);

    int querySurface(EGLSurface eglSurface, int what);

    void setPresentationTime(EGLSurface eglSurface, khronos_stime_nanoseconds_t nanoseconds);

    EGLContext getContext();

    EGLDisplay getDisplay();

    EGLConfig getConfig();

};

#endif //JNIDE_EGL_CORE_H
