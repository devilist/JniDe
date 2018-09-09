APP_ABI := armeabi-v7a
#选择stl运行时库 默认为系统最小库 system
APP_STL := gnustl_static
APP_CPPFLAGS := -std=gnu++11 -fexceptions -D__STDC_LIMIT_MACROS
NDK_TOOLCHAIN_VERSION = 4.9
APP_PLATFORM := android-14