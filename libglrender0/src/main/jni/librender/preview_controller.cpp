//
// Created by admin on 2018/10/22.
//

#include "include/preview_controller.h"
#include "../libutils/my_log.h"

#define LOG_TAG "PreviewController"

PreviewController::PreviewController() {
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

PreviewController::~PreviewController() {
	MY_LOGE(LOG_TAG, "PreviewController destroyed");
	pthread_mutex_destroy(&pLock);
	pthread_cond_destroy(&pCondition);
}

void PreviewController::setWindow(ANativeWindow *window) {
	pthread_mutex_lock(&pLock);
	renderMsg = MSG_WINDOW_SET;
	nativeWindow = window;
	pthread_cond_signal(&pCondition);
	pthread_mutex_unlock(&pLock);
}

void PreviewController::resetSize(int width, int height) {

}

bool PreviewController::start(char *filePath) {
	return false;
}

void PreviewController::stop() {

}

void *PreviewController::threadStartCallback(void *myself) {
	return nullptr;
}

void PreviewController::renderLoop() {

}

bool PreviewController::initialize() {
	return false;
}

void PreviewController::updateTextImage() {

}

void PreviewController::drawFrame() {

}

void PreviewController::destroy() {

}
