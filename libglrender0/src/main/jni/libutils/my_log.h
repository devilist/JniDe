//
// Created by zengp on 2018/4/3.
//
//#pragma once
#ifndef HELLO_JNI_LOGUTIL_H
#define HELLO_JNI_LOGUTIL_H

#include <android/log.h>

#define MY_LOG_LEVEL_VERBOSE    1
#define MY_LOG_LEVEL_DEBUG      2
#define MY_LOG_LEVEL_INFO       3
#define MY_LOG_LEVEL_WARNING    4
#define MY_LOG_LEVEL_ERROR      5
#define MY_LOG_LEVEL_FATAL      6
#define MY_LOG_LEVEL_SILENT     7

#ifndef MY_LOG_TAG
//  __FILE__  文件路径
    #define MY_LOG_TAG __FILE__
#endif

#ifndef MY_LOG_LEVEL
    #define MY_LOG_LEVEL MY_LOG_LEVEL_VERBOSE
#endif

#define MY_LOG_NOOP(void) 0

#define MY_LOG_PRINT(level, tag, fmt, ...) \
        __android_log_print(level,                                    \
                            tag,                                      \
                            " \n    ==== NDK LOG ==== "               \
                            "line %u at : %s; function : %s;\n"       \
                            "   " fmt,                                \
                            __LINE__, __FILE__, __PRETTY_FUNCTION__,  \
                            ##__VA_ARGS__)

#if MY_LOG_LEVEL <= MY_LOG_LEVEL_VERBOSE
    #define MY_LOGV(tag, fmt, ...) \
            MY_LOG_PRINT(ANDROID_LOG_VERBOSE, tag, fmt, ##__VA_ARGS__)
#else
    #define MY_LOGV(...) MY_LOG_NOOP()
#endif

#if MY_LOG_LEVEL <= MY_LOG_LEVEL_DEBUG
    #define MY_LOGD(tag, fmt, ...) \
            MY_LOG_PRINT(ANDROID_LOG_DEBUG, tag, fmt, ##__VA_ARGS__)
#else
    #define MY_LOGD(...) MY_LOG_NOOP()
#endif

#if MY_LOG_LEVEL <= MY_LOG_LEVEL_INFO
    #define MY_LOGI(tag, fmt, ...) \
            MY_LOG_PRINT(ANDROID_LOG_INFO, tag, fmt, ##__VA_ARGS__)
#else
    #define MY_LOGI(...) MY_LOG_NOOP()
#endif

#if MY_LOG_LEVEL <= MY_LOG_LEVEL_WARNING
    #define MY_LOGW(tag, fmt, ...) \
            MY_LOG_PRINT(ANDROID_LOG_WARN, tag, fmt, ##__VA_ARGS__)
#else
    #define MY_LOGW(...) MY_LOG_NOOP()
#endif

#if MY_LOG_LEVEL <= MY_LOG_LEVEL_ERROR
    #define MY_LOGE(tag, fmt, ...) \
            MY_LOG_PRINT(ANDROID_LOG_ERROR, tag, fmt, ##__VA_ARGS__)
#else
    #define MY_LOGE(...) MY_LOG_NOOP()
#endif

#if MY_LOG_LEVEL <= MY_LOG_LEVEL_FATAL
    #define MY_LOGF(tag, fmt, ...) \
            MY_LOG_PRINT(ANDROID_LOG_FATAL, tag, fmt, ##__VA_ARGS__)
#else
    #define MY_LOGF(...) MY_LOG_NOOP()
#endif

// expression 为需要断言的表达式
#if MY_LOG_LEVEL <= MY_LOG_LEVEL_FATAL
    #define MY_LOG_ASSERT(tag, expression, fmt, ...) \
        if (!(expression))                                              \
        {                                                               \
        __android_log_assert(#expression, tag, fmt, ##__VA_ARGS__); \
        }
#else
    #define MY_LOG_ASSERT(...) MY_LOG_NOOP()
#endif

#endif //HELLO_JNI_LOGUTIL_H
