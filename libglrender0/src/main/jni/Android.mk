LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := pngdecoder
LOCAL_SRC_FILES := $(LOCAL_PATH)/libpngdecoder/libpngdecoder.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
# -D 在所有源文件里增加一个宏定义 #define __STDC_CONSTANT_MACROS
LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/libpngdecoder/include

LOCAL_STATIC_LIBRARIES := pngdecoder
LOCAL_SRC_FILES = \
	$(LOCAL_PATH)/librender/egl_core.cpp \
	$(LOCAL_PATH)/librender/preview_texture.cpp \
	$(LOCAL_PATH)/librender/preview_render.cpp \
	$(LOCAL_PATH)/librender/preview_controller.cpp

#日志等级
ifeq ($(APP_OPTIM), release)
    MY_LOG_LEVEL := MY_LOG_LEVEL_ERROR
else
    MY_LOG_LEVEL := MY_LOG_LEVEL_DEBUG
endif
LOCAL_CFLAGS += -DMY_LOG_LEVEL=$(MY_LOG_LEVEL)

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -lz -landroid
# Link with OpenGL ES
LOCAL_LDLIBS += -lGLESv2 -lEGL

LOCAL_MODULE := pngrender
include $(BUILD_SHARED_LIBRARY)

