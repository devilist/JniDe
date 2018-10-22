//
// Created by zengp on 2018/10/21.
//

#ifndef JNIDE_PREVIEW_CONTROLLER_H
#define JNIDE_PREVIEW_CONTROLLER_H


#include <pthread.h>
#include <unistd.h>
#include <queue>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/native_window.h>

#include "egl_core.h"
#include "preview_texture.h"
#include "preview_render.h"

#include "../../libpngdecoder/include/png_decoder.h"

class PreviewController {
public:

	PreviewController();

	virtual ~PreviewController();

	void setWindow(ANativeWindow *window);

	void resetSize(int width, int height);

	bool start(char *filePath);

	void stop();

private:

	enum RenderThreadMessage {
		MSG_NONE = 0, MSG_WINDOW_SET, MSG_RENDER_LOOP_EXIT
	};

	int screenWidth, screenHeight;

	// window
	ANativeWindow *nativeWindow;
	EGLSurface previewSurface;
	EGLCore *eglCore;

	// gl
	PreviewRender *render;
	PreviewTexture *texture;

	// png
	PngPicDecoder *pngDecoder;

	// thread
	pthread_t pthread;
	pthread_mutex_t pLock;
	pthread_cond_t pCondition;
	enum RenderThreadMessage renderMsg;

	// 回调函数
	static void *threadStartCallback(void *myself);

	void renderLoop();

	bool initialize();

	void updateTextImage();

	void drawFrame();

	void destroy();
};


#endif //JNIDE_PREVIEW_CONTROLLER_H
