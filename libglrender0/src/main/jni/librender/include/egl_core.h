//
// Created by zengp on 2018/9/22.
//

#ifndef JNIDE_EGL_CORE_H
#define JNIDE_EGL_CORE_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/native_window.h>
#include <KHR/khrplatform.h>  // https://stackoverflow.com/questions/49683210/what-is-the-khr-platform-header-doing

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

    EGLSurface createWindowSurface(ANativeWindow *_window);

    EGLSurface createOffScreenSurface(int width, int height);

    bool makeCurrent(EGLSurface eglSurface);

    bool swapBuffers(EGLSurface eglSurface);

    void releaseSurface(EGLSurface eglSurface);

    void release();

    int querySurface(EGLSurface eglSurface, int what);

    int setPresentationTime(EGLSurface eglSurface, khronos_stime_nanoseconds_t nanoseconds);

    EGLContext getContext();

    EGLDisplay getDisplay();

    EGLConfig getConfig();

};

#endif //JNIDE_EGL_CORE_H
