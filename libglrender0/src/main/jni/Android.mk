LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := pngdecoder
LOCAL_SRC_FILES := $(LOCAL_PATH)/libpngdecoder/libpngdecoder.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/libpngdecoder/include

LOCAL_STATIC_LIBRARIES := pngdecoder
LOCAL_SRC_FILES = \
	$(LOCAL_PATH)/librender/egl_core.cpp

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -lz -landroid
# Link with OpenGL ES
LOCAL_LDLIBS += -lGLESv2 -lEGL

LOCAL_MODULE := pngrender
include $(BUILD_SHARED_LIBRARY)

