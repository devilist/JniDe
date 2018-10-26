//
// Created by admin on 2018/10/25.
//

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "librender/include/preview_controller.h"

#include "com_devilist_glrender_pngrender_PngPreviewController0.h"

#define LOG_TAG "PicPreviewController"

static ANativeWindow *nativeWindow = 0;
static PreviewController *previewController = 0;

JNIEXPORT void JNICALL Java_com_devilist_glrender_pngrender_PngPreviewController0_init
		(JNIEnv *env, jobject obj, jstring pngPath) {
	previewController = new PreviewController();
	char *pngFilePath = (char *) env->GetStringUTFChars(pngPath, NULL);
	previewController->start(pngFilePath);
	env->ReleaseStringUTFChars(pngPath, pngFilePath);
}

JNIEXPORT
void JNICALL Java_com_devilist_glrender_pngrender_PngPreviewController0_setSurface
		(JNIEnv *env, jobject obj, jobject surface) {
	if (0 != surface && NULL != previewController) {
		nativeWindow = ANativeWindow_fromSurface(env, surface);
		previewController->setWindow(nativeWindow);
	} else {
		ANativeWindow_release(nativeWindow);
		nativeWindow = 0;
	}
}

JNIEXPORT
void JNICALL Java_com_devilist_glrender_pngrender_PngPreviewController0_resetSize
		(JNIEnv *env, jobject obj, jint width, jint height) {
	if (NULL != previewController) {
		previewController->resetSize(width, height);
	}
}

JNIEXPORT
void JNICALL Java_com_devilist_glrender_pngrender_PngPreviewController0_stop
		(JNIEnv *env, jobject obj) {
	if (NULL != previewController) {
		previewController->stop();
		delete previewController;
		previewController = NULL;
	}
}



