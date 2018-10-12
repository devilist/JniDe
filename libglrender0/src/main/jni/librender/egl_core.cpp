//
// Created by zengp on 2018/9/22.
//

#include "./include/egl_core.h"

EGLCore::EGLCore()
{

}

EGLCore::~EGLCore()
{

}

bool EGLCore::init()
{
    return false;
}

bool EGLCore::init(EGLContext shareContext)
{
    return false;
}

EGLSurface EGLCore::createWindowSurface(ANativeWindow *_window)
{
    return nullptr;
}

EGLSurface EGLCore::createOffScreenSurface(int width, int height)
{
    return nullptr;
}

bool EGLCore::makeCurrent(EGLSurface eglSurface)
{
    return false;
}

bool EGLCore::swapBuffers(EGLSurface eglSurface)
{
    return false;
}

void EGLCore::releaseSurface(EGLSurface eglSurface)
{

}

void EGLCore::release()
{

}

int EGLCore::querySurface(EGLSurface eglSurface, int what)
{
    return 0;
}

int EGLCore::setPresentationTime(EGLSurface eglSurface, khronos_stime_nanoseconds_t nanoseconds)
{
    return 0;
}

EGLContext EGLCore::getContext()
{
    return nullptr;
}

EGLDisplay EGLCore::getDisplay()
{
    return nullptr;
}

EGLConfig EGLCore::getConfig()
{
    return nullptr;
}
